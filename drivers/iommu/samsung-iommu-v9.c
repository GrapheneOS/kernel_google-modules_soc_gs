// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd.
 */

#include <linux/dma-iommu.h>
#include <linux/kmemleak.h>
#include <linux/module.h>
#include <linux/of_iommu.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/sched/clock.h>
#include <linux/slab.h>

#include "samsung-iommu-v9.h"

#define REG_MMU_NUM_CONTEXT			0x0100
#define MMU_NUM_CONTEXT(reg)			((reg) & 0x1F)

#define REG_MMU_NUM_CONTEXT_PER_VM(n)		(0x140 + (n) * 0x4)
#define MMU_NUM_CONTEXT_PER_VM(reg)		((reg) & 0x1F)

#define REG_MMU_ALL_INV_VM			0x8010
#define REG_MMU_RANGE_INV_START_VPN_VM		0x8020
#define REG_MMU_RANGE_INV_END_VPN_AND_TRIG_VM	0x8024

#define MMU_PMMU_INFO_VA_WIDTH(reg)		((reg) & 0x1)
#define MMU_SWALKER_INFO_NUM_PMMU(reg)		((reg) & 0xFFFF)
#define MMU_PMMU_INFO_NUM_STREAM_TABLE(reg)	(((reg) >> 16) & 0xFFFF)

#define FLPD_SHAREABLE_FLAG	BIT(6)
#define SLPD_SHAREABLE_FLAG	BIT(4)
#define DEFAULT_QOS_VALUE	-1
#define DEFAULT_STREAM_NONE	~0U
#define UNUSED_STREAM_INDEX	~0U

#define MMU_STREAM_CFG_MASK(reg)		((reg) & (GENMASK(31, 16) | GENMASK(6, 0)))
#define MMU_STREAM_MATCH_CFG_MASK(reg)		((reg) & (GENMASK(9, 8)))

static struct iommu_ops samsung_sysmmu_ops;
static struct platform_driver samsung_sysmmu_driver_v9;

struct samsung_sysmmu_domain {
	struct iommu_domain domain;
	struct samsung_iommu_log log;
	struct iommu_group *group;
	struct sysmmu_drvdata *vm_sysmmu; /* valid only if vid != 0 */
	/* if vid != 0, domain is an aux domain attached to exactly one device and sysmmu */
	unsigned int vid;
	sysmmu_pte_t *page_table;
	atomic_t *lv2entcnt;
	spinlock_t pgtablelock;	/* spinlock to access pagetable	*/
};

static inline void samsung_iommu_write_event(struct samsung_iommu_log *iommu_log,
					     enum sysmmu_event_type type, unsigned int vid,
					     u64 start, u64 end)
{
	struct sysmmu_log *log;
	unsigned int index = (unsigned int)atomic_inc_return(&iommu_log->index) - 1;

	log = &iommu_log->log[index % iommu_log->len];
	log->time = sched_clock();
	log->type = type;
	log->vid = vid;
	log->start = start;
	log->end = end;
}

#define SYSMMU_EVENT_LOG(data, type, vid)	\
					samsung_iommu_write_event(&(data)->log, type, vid, 0, 0)
#define SYSMMU_EVENT_LOG_RANGE(data, type, vid, s, e)	\
					samsung_iommu_write_event(&(data)->log, type, vid, s, e)

static bool sysmmu_global_init_done;
static DEFINE_MUTEX(sysmmu_global_mutex); /* Global driver mutex */
static struct device sync_dev;
static struct kmem_cache *flpt_cache, *slpt_cache;

static inline u32 __sysmmu_get_hw_version(struct sysmmu_drvdata *data)
{
	return MMU_VERSION_RAW(readl_relaxed(data->sfrbase + REG_MMU_VERSION));
}

static inline u32 __sysmmu_get_num_vm(struct sysmmu_drvdata *data)
{
	return MMU_NUM_CONTEXT(readl_relaxed(data->sfrbase + REG_MMU_NUM_CONTEXT));
}

static inline int __sysmmu_verify_num_context_per_vm(struct sysmmu_drvdata *data)
{
	int i;
	int ret = 0;

	for (i = 0; i < data->max_vm; i++) {
		unsigned int num_context;

		num_context = MMU_NUM_CONTEXT_PER_VM(readl_relaxed(data->sfrbase +
						     REG_MMU_NUM_CONTEXT_PER_VM(i)));
		if (num_context == 0) {
			dev_err(data->dev, "No contexts allocated for vm %d", i);
			ret = -ENODEV;
		}
		if (num_context > 1) {
			dev_warn(data->dev,
				 "%u contexts allocated for vm %d (expected only 1 context)",
				 num_context, i);
		}
	}

	return ret;
}

static inline u32 __sysmmu_get_num_pmmu(struct sysmmu_drvdata *data)
{
	return MMU_SWALKER_INFO_NUM_PMMU(readl_relaxed(data->sfrbase + REG_MMU_SWALKER_INFO));
}

static inline void __sysmmu_write_all_vm(struct sysmmu_drvdata *data, u32 value,
					 void __iomem *addr)
{
	int i;

	for (i = 0; i < data->max_vm; i++) {
		if (!(data->vmid_mask & BIT(i)))
			continue;
		writel_relaxed(value, MMU_VM_ADDR(addr, i));
	}
}

static inline void __sysmmu_invalidate_all(struct sysmmu_drvdata *data)
{
	__sysmmu_write_all_vm(data, 0x1, data->sfrbase + REG_MMU_ALL_INV_VM);

	SYSMMU_EVENT_LOG(data, SYSMMU_EVENT_INVALIDATE_ALL, 0);
}

static inline void __sysmmu_invalidate_all_vid(struct sysmmu_drvdata *data, unsigned int vid)
{
	writel_relaxed(0x1, MMU_VM_ADDR(data->sfrbase + REG_MMU_ALL_INV_VM, vid));

	SYSMMU_EVENT_LOG(data, SYSMMU_EVENT_INVALIDATE_ALL, vid);
}

static inline void __sysmmu_invalidate(struct sysmmu_drvdata *data,
				       dma_addr_t start, dma_addr_t end)
{
	__sysmmu_write_all_vm(data, (ALIGN_DOWN(start, SPAGE_SIZE) >> 4),
			      data->sfrbase + REG_MMU_RANGE_INV_START_VPN_VM);
	__sysmmu_write_all_vm(data, ((ALIGN_DOWN(end, (SPAGE_SIZE)) >> 4) | 0x1),
			      data->sfrbase + REG_MMU_RANGE_INV_END_VPN_AND_TRIG_VM);

	SYSMMU_EVENT_LOG_RANGE(data, SYSMMU_EVENT_INVALIDATE_RANGE, 0, start, end);
}

static inline void __sysmmu_invalidate_vid(struct sysmmu_drvdata *data, unsigned int vid,
					   dma_addr_t start, dma_addr_t end)
{
	writel_relaxed(ALIGN_DOWN(start, SPAGE_SIZE) >> 4,
		       MMU_VM_ADDR(data->sfrbase + REG_MMU_RANGE_INV_START_VPN_VM, vid));
	writel_relaxed((ALIGN_DOWN(end, SPAGE_SIZE) >> 4) | 0x1,
		       MMU_VM_ADDR(data->sfrbase + REG_MMU_RANGE_INV_END_VPN_AND_TRIG_VM, vid));

	SYSMMU_EVENT_LOG_RANGE(data, SYSMMU_EVENT_INVALIDATE_RANGE, vid, start, end);
}

static inline void __sysmmu_disable(struct sysmmu_drvdata *data)
{
	__sysmmu_write_all_vm(data, MMU_CTRL_DISABLE, data->sfrbase + REG_MMU_CTRL_VM);
	__sysmmu_invalidate_all(data);

	SYSMMU_EVENT_LOG(data, SYSMMU_EVENT_DISABLE, 0);
}

static inline void __sysmmu_disable_vid(struct sysmmu_drvdata *data, unsigned int vid)
{
	writel_relaxed(MMU_CTRL_DISABLE,
		       MMU_VM_ADDR(data->sfrbase + REG_MMU_CTRL_VM, vid));
	writel_relaxed(0, MMU_VM_ADDR(data->sfrbase + REG_MMU_CONTEXT0_CFG_FLPT_BASE_VM, vid));
	__sysmmu_invalidate_all_vid(data, vid);

	SYSMMU_EVENT_LOG(data, SYSMMU_EVENT_DISABLE, vid);
}

static inline void __sysmmu_set_stream(struct sysmmu_drvdata *data, int pmmu_id)
{
	struct stream_props *props = &data->props[pmmu_id];
	struct stream_config *cfg = props->cfg;
	int id_cnt = props->id_cnt;
	unsigned int i, index;

	writel_relaxed(MMU_SET_PMMU_INDICATOR(pmmu_id), data->sfrbase + REG_MMU_PMMU_INDICATOR);
	readl_relaxed(data->sfrbase + REG_MMU_PMMU_INDICATOR);

	writel_relaxed(MMU_STREAM_CFG_MASK(props->default_cfg),
		       data->sfrbase + REG_MMU_STREAM_CFG(0));

	for (i = 0; i < id_cnt; i++) {
		if (cfg[i].index == UNUSED_STREAM_INDEX)
			continue;

		index = cfg[i].index;
		writel_relaxed(MMU_STREAM_CFG_MASK(cfg[i].cfg),
			       data->sfrbase + REG_MMU_STREAM_CFG(index));
		writel_relaxed(MMU_STREAM_MATCH_CFG_MASK(cfg[i].match_cfg),
			       data->sfrbase + REG_MMU_STREAM_MATCH_CFG(index));
		writel_relaxed(cfg[i].match_id_value,
			       data->sfrbase + REG_MMU_STREAM_MATCH_SID_VALUE(index));
		writel_relaxed(cfg[i].match_id_mask,
			       data->sfrbase + REG_MMU_STREAM_MATCH_SID_MASK(index));
	}
}

static inline void __sysmmu_init_config_attribute(struct sysmmu_drvdata *data, unsigned int vid)
{
	u32 cfg;

	cfg = readl_relaxed(MMU_VM_ADDR(data->sfrbase + REG_MMU_CONTEXT0_CFG_ATTRIBUTE_VM, vid));

	cfg &= ~CFG_PT_CACHEABLE_MASK;
	cfg |= CFG_PT_CACHEABLE_NORMAL_NC;

	if (data->qos != DEFAULT_QOS_VALUE) {
		cfg &= ~CFG_QOS(0xF);
		cfg |= CFG_QOS_OVRRIDE | CFG_QOS(data->qos);
	}
	writel_relaxed(cfg, MMU_VM_ADDR(data->sfrbase + REG_MMU_CONTEXT0_CFG_ATTRIBUTE_VM, vid));
}

static inline void __sysmmu_init_config(struct sysmmu_drvdata *data)
{
	int i;

	for (i = 0; i < data->max_vm; i++) {
		if (!(data->vmid_mask & BIT(i)))
			continue;
		__sysmmu_init_config_attribute(data, i);
	}

	for (i = 0; i < data->num_pmmu; i++)
		__sysmmu_set_stream(data, i);
}

static inline void __sysmmu_enable_vid(struct sysmmu_drvdata *data, unsigned int vid)
{
	writel_relaxed(data->pgtable[vid] / SPAGE_SIZE,
		       MMU_VM_ADDR(data->sfrbase + REG_MMU_CONTEXT0_CFG_FLPT_BASE_VM, vid));
	writel_relaxed(MMU_CTRL_ENABLE,
		       MMU_VM_ADDR(data->sfrbase + REG_MMU_CTRL_VM, vid));
	__sysmmu_init_config_attribute(data, vid);
	__sysmmu_invalidate_all_vid(data, vid);

	SYSMMU_EVENT_LOG(data, SYSMMU_EVENT_ENABLE, vid);
}

static inline void __sysmmu_enable(struct sysmmu_drvdata *data)
{
	__sysmmu_write_all_vm(data, MMU_CTRL_ENABLE, data->sfrbase + REG_MMU_CTRL_VM);
	__sysmmu_write_all_vm(data, data->pgtable[0] / SPAGE_SIZE,
			      data->sfrbase + REG_MMU_CONTEXT0_CFG_FLPT_BASE_VM);
	__sysmmu_init_config(data);
	__sysmmu_invalidate_all(data);

	SYSMMU_EVENT_LOG(data, SYSMMU_EVENT_ENABLE, 0);
}

static struct samsung_sysmmu_domain *to_sysmmu_domain(struct iommu_domain *dom)
{
	return container_of(dom, struct samsung_sysmmu_domain, domain);
}

static inline void pgtable_flush(void *vastart, void *vaend)
{
	dma_sync_single_for_device(&sync_dev, virt_to_phys(vastart),
				   vaend - vastart, DMA_TO_DEVICE);
}

static bool samsung_sysmmu_capable(enum iommu_cap cap)
{
	return cap == IOMMU_CAP_CACHE_COHERENCY;
}

static inline size_t get_log_fit_pages(struct samsung_iommu_log *log, int len)
{
	return DIV_ROUND_UP(sizeof(*log->log) * len, PAGE_SIZE);
}

static void samsung_iommu_deinit_log(struct samsung_iommu_log *log)
{
	struct page *page;
	unsigned long i;
	size_t fit_pages = get_log_fit_pages(log, log->len);

	page = virt_to_page(log->log);
	for (i = 0; i < fit_pages; i++)
		__free_page(page + i);
}

static int samsung_iommu_init_log(struct samsung_iommu_log *log, int len)
{
	struct page *page;
	int order;
	unsigned long order_pages;
	size_t fit_pages = get_log_fit_pages(log, len);

	atomic_set(&log->index, 0);
	order = get_order(fit_pages * PAGE_SIZE);
	page = alloc_pages(GFP_KERNEL | __GFP_ZERO, order);
	if (!page)
		return -ENOMEM;

	split_page(page, order);

	order_pages = 1 << order;
	if (order_pages > fit_pages) {
		unsigned long i;

		for (i = fit_pages; i < order_pages; i++)
			__free_page(page + i);
	}

	log->log = page_address(page);
	log->len = len;

	return 0;
}

static struct iommu_domain *samsung_sysmmu_domain_alloc(unsigned int type)
{
	struct samsung_sysmmu_domain *domain;

	if (type != IOMMU_DOMAIN_UNMANAGED &&
	    type != IOMMU_DOMAIN_DMA &&
	    type != IOMMU_DOMAIN_IDENTITY) {
		pr_err("invalid domain type %u\n", type);
		return NULL;
	}

	domain = kzalloc(sizeof(*domain), GFP_KERNEL);
	if (!domain)
		return NULL;

	domain->page_table = (sysmmu_pte_t *)kmem_cache_alloc(flpt_cache, GFP_KERNEL | __GFP_ZERO);
	if (!domain->page_table)
		goto err_pgtable;

	domain->lv2entcnt = kcalloc(NUM_LV1ENTRIES, sizeof(*domain->lv2entcnt), GFP_KERNEL);
	if (!domain->lv2entcnt)
		goto err_counter;

	if (type == IOMMU_DOMAIN_DMA) {
		int ret = iommu_get_dma_cookie(&domain->domain);

		if (ret) {
			pr_err("failed to get dma cookie (%d)\n", ret);
			goto err_get_dma_cookie;
		}
	}

	if (samsung_iommu_init_log(&domain->log, SYSMMU_EVENT_MAX)) {
		pr_err("failed to init domian logging\n");
		goto err_init_log;
	}

	pgtable_flush(domain->page_table, domain->page_table + NUM_LV1ENTRIES);

	spin_lock_init(&domain->pgtablelock);

	return &domain->domain;

err_init_log:
	iommu_put_dma_cookie(&domain->domain);
err_get_dma_cookie:
	kfree(domain->lv2entcnt);
err_counter:
	kmem_cache_free(flpt_cache, domain->page_table);
err_pgtable:
	kfree(domain);

	return NULL;
}

static void samsung_sysmmu_domain_free(struct iommu_domain *dom)
{
	struct samsung_sysmmu_domain *domain = to_sysmmu_domain(dom);

	samsung_iommu_deinit_log(&domain->log);
	iommu_put_dma_cookie(dom);
	kmem_cache_free(flpt_cache, domain->page_table);
	kfree(domain->lv2entcnt);
	kfree(domain);
}

static inline void samsung_sysmmu_detach_drvdata(struct sysmmu_drvdata *data)
{
	unsigned long flags;

	spin_lock_irqsave(&data->lock, flags);
	if (--data->attached_count == 0) {
		if (pm_runtime_active(data->dev))
			__sysmmu_disable(data);

		list_del(&data->list);
		data->pgtable[0] = 0;
		data->group = NULL;
	}
	spin_unlock_irqrestore(&data->lock, flags);
}

/* TODO: Since upstream has removed of_get_dma_window() we'll temporarily
 * open code relevant portion here, until we can come up with a generic
 * enough implementation that can be proposed upstream for these drivers.
 */
static int samsung_sysmmu_get_dma_window(struct device_node *dn,
					 dma_addr_t *addr, size_t *size)
{
	const __be32 *dma_window, *prop;
	u32 cells;

	dma_window = of_get_property(dn, "dma-window", NULL);
	if (!dma_window)
		return -ENODEV;

	prop = of_get_property(dn, "#dma-address-cells", NULL);
	cells = prop ? be32_to_cpup(prop) : of_n_addr_cells(dn);
	if (!cells)
		return -EINVAL;
	*addr = of_read_number(dma_window, cells);
	dma_window += cells;

	prop = of_get_property(dn, "#dma-size-cells", NULL);
	cells = prop ? be32_to_cpup(prop) : of_n_size_cells(dn);
	if (!cells)
		return -EINVAL;
	*size = of_read_number(dma_window, cells);

	return 0;
}

static int samsung_sysmmu_set_domain_range(struct iommu_domain *dom,
					   struct device *dev)
{
	struct iommu_domain_geometry *geom = &dom->geometry;
	dma_addr_t start, end;
	size_t size;

	if (samsung_sysmmu_get_dma_window(dev->of_node, &start, &size))
		return 0;

	end = start + size;

	if (end > DMA_BIT_MASK(32))
		end = DMA_BIT_MASK(32);

	if (geom->force_aperture) {
		dma_addr_t d_start, d_end;

		d_start = max(start, geom->aperture_start);
		d_end = min(end, geom->aperture_end);

		if (d_start >= d_end) {
			dev_err(dev, "current range is [%pad..%pad]\n",
				&geom->aperture_start, &geom->aperture_end);
			dev_err(dev, "requested range [%zx @ %pad] is not allowed\n",
				size, &start);
			return -ERANGE;
		}

		geom->aperture_start = d_start;
		geom->aperture_end = d_end;
	} else {
		geom->aperture_start = start;
		geom->aperture_end = end;
		/*
		 * All CPUs should observe the change of force_aperture after
		 * updating aperture_start and aperture_end because dma-iommu
		 * restricts dma virtual memory by this aperture when
		 * force_aperture is set.
		 * We allow allocating dma virtual memory during changing the
		 * aperture range because the current allocation is free from
		 * the new restricted range.
		 */
		smp_wmb();
		geom->force_aperture = true;
	}

	dev_info(dev, "changed DMA range [%pad..%pad] successfully.\n",
		 &geom->aperture_start, &geom->aperture_end);

	return 0;
}

static struct samsung_sysmmu_domain *attach_helper(struct iommu_domain *dom, struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	struct samsung_sysmmu_domain *domain;

	if (!fwspec || fwspec->ops != &samsung_sysmmu_ops) {
		dev_err(dev, "failed to attach, IOMMU instance data %s.\n",
			!fwspec ? "is not initialized" : "has different ops");
		return ERR_PTR(-ENXIO);
	}

	if (!dev_iommu_priv_get(dev)) {
		dev_err(dev, "has no IOMMU\n");
		return ERR_PTR(-ENODEV);
	}

	domain = to_sysmmu_domain(dom);
	if (domain->vm_sysmmu) {
		dev_err(dev, "IOMMU domain is already used as AUX domain\n");
		return ERR_PTR(-EBUSY);
	}

	return domain;
}

static int samsung_sysmmu_attach_dev(struct iommu_domain *dom, struct device *dev)
{
	struct sysmmu_clientdata *client;
	struct samsung_sysmmu_domain *domain;
	struct list_head *group_list;
	struct sysmmu_drvdata *drvdata;
	struct iommu_group *group = dev->iommu_group;
	unsigned long flags;
	phys_addr_t page_table;
	int i, ret = -EINVAL;

	domain = attach_helper(dom, dev);
	if (IS_ERR(domain))
		return (int)PTR_ERR(domain);

	domain->group = group;
	group_list = iommu_group_get_iommudata(group);
	page_table = virt_to_phys(domain->page_table);

	client = dev_iommu_priv_get(dev);
	for (i = 0; i < client->sysmmu_count; i++) {
		drvdata = client->sysmmus[i];

		spin_lock_irqsave(&drvdata->lock, flags);
		if (drvdata->attached_count++ == 0) {
			list_add(&drvdata->list, group_list);
			drvdata->group = group;
			drvdata->pgtable[0] = page_table;

			if (pm_runtime_active(drvdata->dev))
				__sysmmu_enable(drvdata);
		} else if (drvdata->pgtable[0] != page_table) {
			dev_err(dev, "%s is already attached to other domain\n",
				dev_name(drvdata->dev));
			spin_unlock_irqrestore(&drvdata->lock, flags);
			goto err_drvdata_add;
		}
		spin_unlock_irqrestore(&drvdata->lock, flags);
	}

	ret = samsung_sysmmu_set_domain_range(dom, dev);
	if (ret)
		goto err_drvdata_add;

	dev_info(dev, "attached with pgtable %pap\n", &page_table);

	return 0;

err_drvdata_add:
	while (i-- > 0) {
		drvdata = client->sysmmus[i];

		samsung_sysmmu_detach_drvdata(drvdata);
	}

	return ret;
}

static void samsung_sysmmu_detach_dev(struct iommu_domain *dom, struct device *dev)
{
	struct sysmmu_clientdata *client;
	struct samsung_sysmmu_domain *domain;
	struct sysmmu_drvdata *drvdata;
	int i;

	domain = to_sysmmu_domain(dom);

	client = dev_iommu_priv_get(dev);
	for (i = 0; i < client->sysmmu_count; i++) {
		drvdata = client->sysmmus[i];
		samsung_sysmmu_detach_drvdata(drvdata);
	}

	dev_info(dev, "detached from pgtable %pa\n", &domain->page_table);
}

static inline sysmmu_pte_t make_sysmmu_pte(phys_addr_t paddr, int pgsize, int attr)
{
	return ((sysmmu_pte_t)((paddr) >> PG_ENT_SHIFT)) | pgsize | attr;
}

static sysmmu_pte_t *alloc_lv2entry(struct samsung_sysmmu_domain *domain,
				    sysmmu_pte_t *sent, sysmmu_iova_t iova,
				    atomic_t *pgcounter)
{
	if (lv1ent_section(sent)) {
		WARN(1, "trying mapping on %#011llx mapped with 1MiB page", iova);
		return ERR_PTR(-EADDRINUSE);
	}

	if (lv1ent_unmapped(sent)) {
		unsigned long flags;
		sysmmu_pte_t *pent;

		pent = kmem_cache_zalloc(slpt_cache, GFP_KERNEL);
		if (!pent)
			return ERR_PTR(-ENOMEM);

		spin_lock_irqsave(&domain->pgtablelock, flags);
		if (lv1ent_unmapped(sent)) {
			*sent = make_sysmmu_pte(virt_to_phys(pent),
						SLPD_FLAG, 0);
			kmemleak_ignore(pent);
			atomic_set(pgcounter, 0);
			pgtable_flush(pent, pent + NUM_LV2ENTRIES);
			pgtable_flush(sent, sent + 1);
		} else {
			/* allocated entry is not used, so free it. */
			kmem_cache_free(slpt_cache, pent);
		}
		spin_unlock_irqrestore(&domain->pgtablelock, flags);
	}

	return page_entry(sent, iova);
}

static inline void clear_lv2_page_table(sysmmu_pte_t *ent, int n)
{
	memset(ent, 0, sizeof(*ent) * n);
}

static int lv1set_section(struct samsung_sysmmu_domain *domain,
			  sysmmu_pte_t *sent, sysmmu_iova_t iova,
			  phys_addr_t paddr, int prot, atomic_t *pgcnt)
{
	int attr = !!(prot & IOMMU_CACHE) ? FLPD_SHAREABLE_FLAG : 0;
	sysmmu_pte_t *pent_to_free = NULL;

	if (lv1ent_section(sent)) {
		WARN(1, "Trying mapping 1MB@%#011llx on valid FLPD", iova);
		return -EADDRINUSE;
	}

	if (lv1ent_page(sent)) {
		if (WARN_ON(atomic_read(pgcnt) != 0)) {
			WARN(1, "Trying mapping 1MB@%#011llx on valid SLPD", iova);
			return -EADDRINUSE;
		}

		pent_to_free = page_entry(sent, 0);
		atomic_set(pgcnt, NUM_LV2ENTRIES);
	}

	*sent = make_sysmmu_pte(paddr, SECT_FLAG, attr);
	pgtable_flush(sent, sent + 1);

	if (pent_to_free) {
		struct iommu_iotlb_gather gather = {
			.start = iova,
			.end = iova + SECT_SIZE - 1,
		};

		iommu_iotlb_sync(&domain->domain, &gather);
		kmem_cache_free(slpt_cache, pent_to_free);
	}

	return 0;
}

static int lv2set_page(sysmmu_pte_t *pent, phys_addr_t paddr,
		       size_t size, int prot, atomic_t *pgcnt)
{
	int attr = !!(prot & IOMMU_CACHE) ? SLPD_SHAREABLE_FLAG : 0;

	if (size == SPAGE_SIZE) {
		if (WARN_ON(!lv2ent_unmapped(pent)))
			return -EADDRINUSE;

		*pent = make_sysmmu_pte(paddr, SPAGE_FLAG, attr);
		pgtable_flush(pent, pent + 1);
		atomic_inc(pgcnt);
	} else {	/* size == LPAGE_SIZE */
		unsigned long i;

		for (i = 0; i < SPAGES_PER_LPAGE; i++, pent++) {
			if (WARN_ON(!lv2ent_unmapped(pent))) {
				clear_lv2_page_table(pent - i, i);
				return -EADDRINUSE;
			}

			*pent = make_sysmmu_pte(paddr, LPAGE_FLAG, attr);
		}
		pgtable_flush(pent - SPAGES_PER_LPAGE, pent);
		atomic_add(SPAGES_PER_LPAGE, pgcnt);
	}

	return 0;
}

static int samsung_sysmmu_map(struct iommu_domain *dom, unsigned long l_iova, phys_addr_t paddr,
			      size_t size, int prot, gfp_t unused)
{
	struct samsung_sysmmu_domain *domain = to_sysmmu_domain(dom);
	sysmmu_iova_t iova = (sysmmu_iova_t)l_iova;
	atomic_t *lv2entcnt = &domain->lv2entcnt[lv1ent_offset(iova)];
	sysmmu_pte_t *entry;
	int ret = -ENOMEM;

	/* Do not use IO coherency if iOMMU_PRIV exists */
	if (!!(prot & IOMMU_PRIV))
		prot &= ~IOMMU_CACHE;

	entry = section_entry(domain->page_table, iova);

	if (size == SECT_SIZE) {
		ret = lv1set_section(domain, entry, iova, paddr, prot, lv2entcnt);
	} else {
		sysmmu_pte_t *pent;

		pent = alloc_lv2entry(domain, entry, iova, lv2entcnt);

		if (IS_ERR(pent))
			ret = PTR_ERR(pent);
		else
			ret = lv2set_page(pent, paddr, size, prot, lv2entcnt);
	}

	if (ret)
		pr_err("failed to map %#zx @ %#llx, ret:%d\n", size, iova, ret);
	else
		SYSMMU_EVENT_LOG_RANGE(domain, SYSMMU_EVENT_MAP, domain->vid, iova, iova + size);

	return ret;
}

static size_t samsung_sysmmu_unmap(struct iommu_domain *dom, unsigned long l_iova, size_t size,
				   struct iommu_iotlb_gather *gather)
{
	struct samsung_sysmmu_domain *domain = to_sysmmu_domain(dom);
	sysmmu_iova_t iova = (sysmmu_iova_t)l_iova;
	atomic_t *lv2entcnt = &domain->lv2entcnt[lv1ent_offset(iova)];
	sysmmu_pte_t *sent, *pent;
	size_t err_pgsize;

	sent = section_entry(domain->page_table, iova);

	if (lv1ent_section(sent)) {
		if (WARN_ON(size < SECT_SIZE)) {
			err_pgsize = SECT_SIZE;
			goto err;
		}

		*sent = 0;
		pgtable_flush(sent, sent + 1);
		size = SECT_SIZE;
		goto done;
	}

	if (unlikely(lv1ent_unmapped(sent))) {
		if (size > SECT_SIZE)
			size = SECT_SIZE;
		goto done;
	}

	/* lv1ent_page(sent) == true here */

	pent = page_entry(sent, iova);

	if (unlikely(lv2ent_unmapped(pent))) {
		size = SPAGE_SIZE;
		goto done;
	}

	if (lv2ent_small(pent)) {
		*pent = 0;
		size = SPAGE_SIZE;
		pgtable_flush(pent, pent + 1);
		atomic_dec(lv2entcnt);
		goto done;
	}

	/* lv1ent_large(pent) == true here */
	if (WARN_ON(size < LPAGE_SIZE)) {
		err_pgsize = LPAGE_SIZE;
		goto err;
	}

	clear_lv2_page_table(pent, SPAGES_PER_LPAGE);
	pgtable_flush(pent, pent + SPAGES_PER_LPAGE);
	size = LPAGE_SIZE;
	atomic_sub(SPAGES_PER_LPAGE, lv2entcnt);

done:
	iommu_iotlb_gather_add_page(dom, gather, iova, size);
	SYSMMU_EVENT_LOG_RANGE(domain, SYSMMU_EVENT_UNMAP, domain->vid, iova, iova + size);

	return size;

err:
	pr_err("failed: size(%#zx) @ %#llx is smaller than page size %#zx\n",
	       size, iova, err_pgsize);
	return 0;
}

static void samsung_sysmmu_flush_iotlb_all(struct iommu_domain *dom)
{
	unsigned long flags;
	struct samsung_sysmmu_domain *domain = to_sysmmu_domain(dom);
	struct sysmmu_drvdata *drvdata;

	if (domain->vm_sysmmu) {
		/* Domain is used as AUX domain */
		drvdata = domain->vm_sysmmu;
		spin_lock_irqsave(&drvdata->lock, flags);
		if (drvdata->attached_count && drvdata->rpm_count > 0)
			__sysmmu_invalidate_all_vid(drvdata, domain->vid);
		spin_unlock_irqrestore(&drvdata->lock, flags);
	} else if (domain->group) {
		/* Domain is used as regular domain */
		/*
		 * domain->group might be NULL if flush_iotlb_all is called
		 * before attach_dev. Just ignore it.
		 */
		struct list_head *sysmmu_list = iommu_group_get_iommudata(domain->group);

		list_for_each_entry(drvdata, sysmmu_list, list) {
			spin_lock_irqsave(&drvdata->lock, flags);
			if (drvdata->attached_count && drvdata->rpm_count > 0)
				__sysmmu_invalidate_all(drvdata);
			spin_unlock_irqrestore(&drvdata->lock, flags);
		}
	}
}

static void samsung_sysmmu_iotlb_sync(struct iommu_domain *dom, struct iommu_iotlb_gather *gather)
{
	unsigned long flags;
	struct samsung_sysmmu_domain *domain = to_sysmmu_domain(dom);
	struct sysmmu_drvdata *drvdata;

	if (domain->vm_sysmmu) {
		/* Domain is used as AUX domain */
		drvdata = domain->vm_sysmmu;
		spin_lock_irqsave(&drvdata->lock, flags);
		if (drvdata->attached_count && drvdata->rpm_count > 0)
			__sysmmu_invalidate_vid(drvdata, domain->vid, gather->start, gather->end);
		SYSMMU_EVENT_LOG_RANGE(drvdata, SYSMMU_EVENT_IOTLB_SYNC, domain->vid,
				       gather->start, gather->end);
		spin_unlock_irqrestore(&drvdata->lock, flags);
	} else if (domain->group) {
		/* Domain is used as regular domain */
		/*
		 * domain->group might be NULL if iotlb_sync is called
		 * before attach_dev. Just ignore it.
		 */
		struct list_head *sysmmu_list = iommu_group_get_iommudata(domain->group);

		list_for_each_entry(drvdata, sysmmu_list, list) {
			spin_lock_irqsave(&drvdata->lock, flags);
			if (drvdata->attached_count && drvdata->rpm_count > 0)
				__sysmmu_invalidate(drvdata, gather->start, gather->end);
			SYSMMU_EVENT_LOG_RANGE(drvdata, SYSMMU_EVENT_IOTLB_SYNC, 0,
					       gather->start, gather->end);
			spin_unlock_irqrestore(&drvdata->lock, flags);
		}
	}
}

static phys_addr_t samsung_sysmmu_iova_to_phys(struct iommu_domain *dom, dma_addr_t d_iova)
{
	struct samsung_sysmmu_domain *domain = to_sysmmu_domain(dom);
	sysmmu_iova_t iova = (sysmmu_iova_t)d_iova;
	sysmmu_pte_t *entry;
	phys_addr_t phys = 0;

	entry = section_entry(domain->page_table, iova);

	if (lv1ent_section(entry)) {
		phys = section_phys(entry) + section_offs(iova);
	} else if (lv1ent_page(entry)) {
		entry = page_entry(entry, iova);

		if (lv2ent_large(entry))
			phys = lpage_phys(entry) + lpage_offs(iova);
		else if (lv2ent_small(entry))
			phys = spage_phys(entry) + spage_offs(iova);
	}

	return phys;
}

static struct iommu_device *samsung_sysmmu_probe_device(struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	struct sysmmu_clientdata *client;
	int i;

	if (!fwspec) {
		dev_dbg(dev, "IOMMU instance data is not initialized\n");
		return ERR_PTR(-ENODEV);
	}

	if (fwspec->ops != &samsung_sysmmu_ops) {
		dev_err(dev, "has different IOMMU ops\n");
		return ERR_PTR(-ENODEV);
	}

	client = (struct sysmmu_clientdata *)dev_iommu_priv_get(dev);
	if (client->dev_link) {
		dev_info(dev, "is already added. It's okay.\n");
		return 0;
	}
	client->dev_link = kcalloc(client->sysmmu_count,
				   sizeof(*client->dev_link), GFP_KERNEL);
	if (!client->dev_link)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < client->sysmmu_count; i++) {
		client->dev_link[i] =
			device_link_add(dev, client->sysmmus[i]->dev,
					DL_FLAG_STATELESS | DL_FLAG_PM_RUNTIME);
		if (!client->dev_link[i]) {
			dev_err(dev, "failed to add device link of %s\n",
				dev_name(client->sysmmus[i]->dev));
			while (i-- > 0)
				device_link_del(client->dev_link[i]);
			return ERR_PTR(-EINVAL);
		}
		dev_dbg(dev, "device link to %s\n",
			dev_name(client->sysmmus[i]->dev));
	}

	return &client->sysmmus[0]->iommu;
}

static void samsung_sysmmu_release_device(struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	struct sysmmu_clientdata *client;
	int i;

	if (!fwspec || fwspec->ops != &samsung_sysmmu_ops)
		return;

	client = (struct sysmmu_clientdata *)dev_iommu_priv_get(dev);
	for (i = 0; i < client->sysmmu_count; i++)
		device_link_del(client->dev_link[i]);
	kfree(client->dev_link);

	iommu_fwspec_free(dev);
}

static void samsung_sysmmu_group_data_release(void *iommu_data)
{
	kfree(iommu_data);
}

static struct iommu_group *samsung_sysmmu_device_group(struct device *dev)
{
	struct iommu_group *group;
	struct device_node *np;
	struct platform_device *pdev;
	struct list_head *list;

	if (device_iommu_mapped(dev))
		return iommu_group_get(dev);

	np = of_parse_phandle(dev->of_node, "samsung,iommu-group", 0);
	if (!np) {
		dev_err(dev, "group is not registered\n");
		return ERR_PTR(-ENODEV);
	}

	pdev = of_find_device_by_node(np);
	if (!pdev) {
		dev_err(dev, "no device in device_node[%s]\n", np->name);
		of_node_put(np);
		return ERR_PTR(-ENODEV);
	}

	of_node_put(np);

	group = platform_get_drvdata(pdev);
	if (!group) {
		dev_err(dev, "no group in device_node[%s]\n", np->name);
		return ERR_PTR(-EPROBE_DEFER);
	}

	mutex_lock(&sysmmu_global_mutex);
	if (iommu_group_get_iommudata(group)) {
		mutex_unlock(&sysmmu_global_mutex);
		return group;
	}

	list = kzalloc(sizeof(*list), GFP_KERNEL);
	if (!list) {
		mutex_unlock(&sysmmu_global_mutex);
		return ERR_PTR(-ENOMEM);
	}

	INIT_LIST_HEAD(list);
	iommu_group_set_iommudata(group, list,
				  samsung_sysmmu_group_data_release);

	mutex_unlock(&sysmmu_global_mutex);
	return group;
}

static void samsung_sysmmu_clientdata_release(struct device *dev, void *res)
{
	struct sysmmu_clientdata *client = res;

	kfree(client->sysmmus);
}

static int samsung_sysmmu_of_xlate(struct device *dev, struct of_phandle_args *args)
{
	struct platform_device *sysmmu = of_find_device_by_node(args->np);
	struct sysmmu_drvdata *data = platform_get_drvdata(sysmmu);
	struct sysmmu_drvdata **new_link;
	struct sysmmu_clientdata *client;

	if (!dev_iommu_priv_get(dev)) {
		client = devres_alloc(samsung_sysmmu_clientdata_release,
				      sizeof(*client), GFP_KERNEL);
		if (!client)
			return -ENOMEM;
		dev_iommu_priv_set(dev, client);
		devres_add(&sysmmu->dev, client);
	}

	client = (struct sysmmu_clientdata *)dev_iommu_priv_get(dev);
	new_link = krealloc(client->sysmmus,
			    sizeof(data) * (client->sysmmu_count + 1),
			    GFP_KERNEL);
	if (!new_link)
		return -ENOMEM;

	client->sysmmus = new_link;
	client->sysmmus[client->sysmmu_count++] = data;

	dev_info(dev, "has sysmmu %s (total count:%d)\n",
		 dev_name(data->dev), client->sysmmu_count);

	return 0;
}

static int samsung_sysmmu_aux_attach_dev(struct iommu_domain *dom, struct device *dev)
{
	struct sysmmu_clientdata *client;
	struct samsung_sysmmu_domain *domain;
	struct sysmmu_drvdata *drvdata;
	unsigned long flags;
	unsigned int vid;

	domain = attach_helper(dom, dev);
	if (IS_ERR(domain))
		return (int)PTR_ERR(domain);

	if (domain->group) {
		dev_err(dev, "IOMMU domain is already in use as vid 0 domain\n");
		return -EBUSY;
	}
	client = (struct sysmmu_clientdata *)dev_iommu_priv_get(dev);
	if (client->sysmmu_count != 1) {
		dev_err(dev, "IOMMU AUX domains not supported for devices served by more than one IOMMU\n");
		return -ENXIO;
	}
	drvdata = client->sysmmus[0];
	spin_lock_irqsave(&drvdata->lock, flags);
	if (!drvdata->attached_count) {
		dev_err(dev, "IOMMU needs to be enabled to attach AUX domain\n");
		spin_unlock_irqrestore(&drvdata->lock, flags);
		return -ENXIO;
	}
	for (vid = 1; vid < MAX_VIDS; vid++) {
		if ((drvdata->vmid_mask & BIT(vid)))
			continue;
		if (!drvdata->pgtable[vid])
			break;
	}
	if (vid == MAX_VIDS) {
		dev_err(dev, "Unable to allocate vid for AUX domain\n");
		spin_unlock_irqrestore(&drvdata->lock, flags);
		return -EBUSY;
	}
	drvdata->pgtable[vid] = virt_to_phys(domain->page_table);
	if (pm_runtime_active(drvdata->dev))
		__sysmmu_enable_vid(drvdata, vid);
	spin_unlock_irqrestore(&drvdata->lock, flags);
	domain->vm_sysmmu = drvdata;
	domain->vid = vid;
	return 0;
}

static void samsung_sysmmu_aux_detach_dev(struct iommu_domain *dom, struct device *dev)
{
	struct samsung_sysmmu_domain *domain;
	struct sysmmu_drvdata *drvdata;
	unsigned long flags;
	unsigned int vid;

	domain = to_sysmmu_domain(dom);

	if (WARN_ON(!domain->vm_sysmmu || !domain->vid))
		return;

	drvdata = domain->vm_sysmmu;
	vid = domain->vid;

	spin_lock_irqsave(&drvdata->lock, flags);
	drvdata->pgtable[vid] = 0;
	if (pm_runtime_active(drvdata->dev))
		__sysmmu_disable_vid(drvdata, vid);
	spin_unlock_irqrestore(&drvdata->lock, flags);

	domain->vm_sysmmu = NULL;
	domain->vid = 0;
}

static int samsung_sysmmu_aux_get_pasid(struct iommu_domain *dom, struct device *dev)
{
	struct samsung_sysmmu_domain *domain;

	domain = to_sysmmu_domain(dom);

	if (!domain->vm_sysmmu)
		return -EINVAL;

	return (int)domain->vid;
}

static bool samsung_sysmmu_dev_has_feat(struct device *dev, enum iommu_dev_features f)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	struct sysmmu_clientdata *client;

	if (f != IOMMU_DEV_FEAT_AUX)
		return false;

	client = (struct sysmmu_clientdata *)dev_iommu_priv_get(dev);
	if (!fwspec || !client || fwspec->ops != &samsung_sysmmu_ops)
		return false;

	if (client->sysmmu_count != 1)
		return false;

	return true;
}

static bool samsung_sysmmu_dev_feat_enabled(struct device *dev, enum iommu_dev_features f)
{
	return samsung_sysmmu_dev_has_feat(dev, f);
}

static int samsung_sysmmu_dev_enable_feat(struct device *dev, enum iommu_dev_features f)
{
	if (!samsung_sysmmu_dev_has_feat(dev, f))
		return -EINVAL;
	return 0;
}

static int samsung_sysmmu_dev_disable_feat(struct device *dev, enum iommu_dev_features f)
{
	return -EINVAL;
}

static void samsung_sysmmu_put_resv_regions(struct device *dev,
					    struct list_head *head)
{
	struct iommu_resv_region *entry, *next;

	list_for_each_entry_safe(entry, next, head, list)
		kfree(entry);
}

#define NR_IOMMU_RESERVED_TYPES 2
static int  samsung_sysmmu_get_resv_regions_by_node(struct device_node *np, struct device *dev,
						    struct list_head *head)
{
	const char *propname[NR_IOMMU_RESERVED_TYPES] = {
		"samsung,iommu-identity-map",
		"samsung,iommu-reserved-map"
	};
	enum iommu_resv_type resvtype[NR_IOMMU_RESERVED_TYPES] = {
		IOMMU_RESV_DIRECT, IOMMU_RESV_RESERVED
	};
	int n_addr_cells = of_n_addr_cells(dev->of_node);
	int n_size_cells = of_n_size_cells(dev->of_node);
	int n_all_cells = n_addr_cells + n_size_cells;
	int type;

	for (type = 0; type < 2; type++) {
		const __be32 *prop;
		u64 base, size;
		int i, cnt;

		prop = of_get_property(dev->of_node, propname[type], &cnt);
		if (!prop)
			continue;

		cnt /=  sizeof(u32);
		if (cnt % n_all_cells != 0) {
			dev_err(dev, "Invalid number(%d) of values in %s\n", cnt, propname[type]);
			return -EINVAL;
		}

		for (i = 0; i < cnt; i += n_all_cells) {
			struct iommu_resv_region *region;

			base = of_read_number(prop + i, n_addr_cells);
			size = of_read_number(prop + i + n_addr_cells, n_size_cells);
			if (base & ~dma_get_mask(dev) || (base + size) & ~dma_get_mask(dev)) {
				dev_err(dev, "Unreachable DMA region in %s, [%#llx..%#llx)\n",
					propname[type], base, base + size);
				return -EINVAL;
			}

			region = iommu_alloc_resv_region(base, size, 0, resvtype[type]);
			if (!region)
				return -ENOMEM;

			list_add_tail(&region->list, head);
			dev_info(dev, "Reserved IOMMU mapping [%#llx..%#llx)\n", base, base + size);

		}
	}

	return 0;
}

static void samsung_sysmmu_get_resv_regions(struct device *dev, struct list_head *head)
{
	struct device_node *curr_node, *target_node, *node;
	struct platform_device *pdev;
	int ret;

	target_node = of_parse_phandle(dev->of_node, "samsung,iommu-group", 0);
	if (!target_node) {
		dev_err(dev, "doesn't have iommu-group property\n");
		return;
	}

	for_each_node_with_property(node, "samsung,iommu-group") {
		curr_node = of_parse_phandle(node, "samsung,iommu-group", 0);
		if (!curr_node || curr_node != target_node) {
			of_node_put(curr_node);
			continue;
		}

		pdev = of_find_device_by_node(node);
		if (!pdev) {
			of_node_put(curr_node);
			continue;
		}

		ret = samsung_sysmmu_get_resv_regions_by_node(dev->of_node, &pdev->dev, head);

		put_device(&pdev->dev);
		of_node_put(curr_node);

		if (ret)
			goto err;
	}

	of_node_put(target_node);

	return;
err:
	of_node_put(target_node);
	samsung_sysmmu_put_resv_regions(dev, head);
	INIT_LIST_HEAD(head);
}

static int samsung_sysmmu_def_domain_type(struct device *dev)
{
	struct device_node *np;
	int ret = 0;

	np = of_parse_phandle(dev->of_node, "samsung,iommu-group", 0);
	if (!np) {
		dev_err(dev, "group is not registered\n");
		return 0;
	}

	if (of_property_read_bool(np, "samsung,unmanaged-domain"))
		ret = IOMMU_DOMAIN_UNMANAGED;

	of_node_put(np);

	return ret;
}

static struct iommu_ops samsung_sysmmu_ops = {
	.owner			= THIS_MODULE,
	.capable		= samsung_sysmmu_capable,
	.domain_alloc		= samsung_sysmmu_domain_alloc,
	.domain_free		= samsung_sysmmu_domain_free,
	.attach_dev		= samsung_sysmmu_attach_dev,
	.detach_dev		= samsung_sysmmu_detach_dev,
	.map			= samsung_sysmmu_map,
	.unmap			= samsung_sysmmu_unmap,
	.flush_iotlb_all	= samsung_sysmmu_flush_iotlb_all,
	.iotlb_sync		= samsung_sysmmu_iotlb_sync,
	.iova_to_phys		= samsung_sysmmu_iova_to_phys,
	.probe_device		= samsung_sysmmu_probe_device,
	.release_device		= samsung_sysmmu_release_device,
	.device_group		= samsung_sysmmu_device_group,
	.of_xlate		= samsung_sysmmu_of_xlate,
	.get_resv_regions	= samsung_sysmmu_get_resv_regions,
	.put_resv_regions	= samsung_sysmmu_put_resv_regions,
	.def_domain_type	= samsung_sysmmu_def_domain_type,
	.dev_has_feat		= samsung_sysmmu_dev_has_feat,
	.dev_feat_enabled	= samsung_sysmmu_dev_feat_enabled,
	.dev_enable_feat	= samsung_sysmmu_dev_enable_feat,
	.dev_disable_feat	= samsung_sysmmu_dev_disable_feat,
	.aux_attach_dev		= samsung_sysmmu_aux_attach_dev,
	.aux_detach_dev		= samsung_sysmmu_aux_detach_dev,
	.aux_get_pasid		= samsung_sysmmu_aux_get_pasid,
	.pgsize_bitmap		= SECT_SIZE | LPAGE_SIZE | SPAGE_SIZE,
};

static int sysmmu_get_hw_info(struct sysmmu_drvdata *data)
{
	int ret = 0;

	ret = pm_runtime_get_sync(data->dev);
	if (ret < 0) {
		pm_runtime_put_noidle(data->dev);
		return ret;
	}

	data->version = __sysmmu_get_hw_version(data);
	data->max_vm = __sysmmu_get_num_vm(data);
	data->num_pmmu = __sysmmu_get_num_pmmu(data);
	ret = __sysmmu_verify_num_context_per_vm(data);

	pm_runtime_put(data->dev);

	return ret;
}

static int sysmmu_parse_stream_property(struct device *dev, struct sysmmu_drvdata *drvdata,
					int pmmu_id)
{
	char default_props_name[32];
	char props_name[32];
	struct stream_props *props = &drvdata->props[pmmu_id];
	struct stream_config *cfg;
	int i, readsize, cnt, ret, num_stream;
	u32 pmmu;

	snprintf(default_props_name, sizeof(default_props_name), "pmmu%d,default_stream", pmmu_id);
	snprintf(props_name, sizeof(props_name), "pmmu%d,stream_property", pmmu_id);

	if (of_property_read_u32(dev->of_node, default_props_name, &props->default_cfg))
		props->default_cfg = DEFAULT_STREAM_NONE;

	cnt = of_property_count_elems_of_size(dev->of_node, props_name, sizeof(*cfg));
	if (cnt <= 0)
		return 0;

	cfg = devm_kcalloc(dev, cnt, sizeof(*cfg), GFP_KERNEL);
	if (!cfg)
		return -ENOMEM;

	readsize = cnt * sizeof(*cfg) / sizeof(u32);
	ret = of_property_read_variable_u32_array(dev->of_node, props_name, (u32 *)cfg,
						  readsize, readsize);
	if (ret < 0) {
		dev_err(dev, "failed to get stream property %s, ret %d\n", props_name, ret);
		return ret;
	}

	/* get num stream */
	writel_relaxed(MMU_SET_PMMU_INDICATOR(pmmu_id),
		       drvdata->sfrbase + REG_MMU_PMMU_INDICATOR);
	readl_relaxed(drvdata->sfrbase + REG_MMU_PMMU_INDICATOR);
	pmmu = readl_relaxed(drvdata->sfrbase + REG_MMU_PMMU_INFO);
	num_stream = MMU_PMMU_INFO_NUM_STREAM_TABLE(pmmu);

	for (i = 0; i < cnt; i++) {
		if (cfg[i].index >= num_stream) {
			dev_err(dev, "invalid index %u in %s is ignored. (max:%d)\n",
				cfg[i].index, props_name, num_stream);
			cfg[i].index = UNUSED_STREAM_INDEX;
		}
	}

	props->id_cnt = cnt;
	props->cfg = cfg;

	return 0;
}

static int __sysmmu_secure_irq_init(struct device *sysmmu, struct sysmmu_drvdata *data)
{
	struct platform_device *pdev = to_platform_device(sysmmu);
	int ret;

	ret = platform_get_irq(pdev, 1);
	if (ret <= 0) {
		dev_err(sysmmu, "unable to find secure IRQ resource\n");
		return -EINVAL;
	}
	data->secure_irq = ret;

	ret = devm_request_threaded_irq(sysmmu, data->secure_irq, samsung_sysmmu_irq,
					samsung_sysmmu_irq_thread, IRQF_ONESHOT,
					dev_name(sysmmu), data);
	if (ret) {
		dev_err(sysmmu, "failed to set secure irq handler %d, ret:%d\n",
			data->secure_irq, ret);
		return ret;
	}

	ret = of_property_read_u32(sysmmu->of_node, "sysmmu,secure_base", &data->secure_base);
	if (ret) {
		dev_err(sysmmu, "failed to get secure base address\n");
		return ret;
	}

	return ret;
}

static int sysmmu_parse_dt(struct device *sysmmu, struct sysmmu_drvdata *data)
{
	unsigned int mask;
	int ret, qos = DEFAULT_QOS_VALUE, i;
	struct stream_props *props;

	/* Parsing QoS */
	ret = of_property_read_u32_index(sysmmu->of_node, "qos", 0, &qos);
	if (!ret && qos > 15) {
		dev_err(sysmmu, "Invalid QoS value %d, use default.\n", qos);
		qos = DEFAULT_QOS_VALUE;
	}
	data->qos = qos;

	/* Secure IRQ */
	if (of_find_property(sysmmu->of_node, "sysmmu,secure-irq", NULL)) {
		ret = __sysmmu_secure_irq_init(sysmmu, data);
		if (ret) {
			dev_err(sysmmu, "failed to init secure irq\n");
			return ret;
		}
	}

	/* use async fault mode */
	data->async_fault_mode = of_property_read_bool(sysmmu->of_node, "sysmmu,async-fault");

	data->vmid_mask = SYSMMU_MASK_VMID;
	ret = of_property_read_u32_index(sysmmu->of_node, "vmid_mask", 0, &mask);
	if (!ret && (mask & ((1 << data->max_vm) - 1)))
		data->vmid_mask = mask;

	props = devm_kcalloc(sysmmu, data->num_pmmu, sizeof(*props), GFP_KERNEL);
	if (!props)
		return -ENOMEM;

	data->props = props;

	for (i = 0; i < data->num_pmmu; i++) {
		ret = sysmmu_parse_stream_property(sysmmu, data, i);
		if (ret)
			dev_err(sysmmu, "Failed to parse PMMU %d streams\n", i);
	}
	return ret;
}

static int samsung_sysmmu_init_global(void)
{
	int ret = 0;

	flpt_cache = kmem_cache_create("samsung-iommu-lv1table", LV1TABLE_SIZE,
				       LV1TABLE_SIZE, 0, NULL);
	if (!flpt_cache)
		return -ENOMEM;

	slpt_cache = kmem_cache_create("samsung-iommu-lv2table", LV2TABLE_SIZE, LV2TABLE_SIZE,
				       0, NULL);
	if (!slpt_cache) {
		ret = -ENOMEM;
		goto err_init_slpt_fail;
	}

	bus_set_iommu(&platform_bus_type, &samsung_sysmmu_ops);

	device_initialize(&sync_dev);
	sysmmu_global_init_done = true;

	return 0;

err_init_slpt_fail:
	kmem_cache_destroy(flpt_cache);

	return ret;
}

static int samsung_sysmmu_device_probe(struct platform_device *pdev)
{
	struct sysmmu_drvdata *data;
	struct device *dev = &pdev->dev;
	struct resource *res;
	int irq, ret, err = 0;

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "failed to get resource info\n");
		return -ENOENT;
	}

	data->sfrbase = devm_ioremap_resource(dev, res);
	if (IS_ERR(data->sfrbase))
		return PTR_ERR(data->sfrbase);

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	ret = devm_request_threaded_irq(dev, irq, samsung_sysmmu_irq,
					samsung_sysmmu_irq_thread,
					IRQF_ONESHOT, dev_name(dev), data);
	if (ret) {
		dev_err(dev, "unabled to register handler of irq %d\n", irq);
		return ret;
	}

	data->clk = devm_clk_get(dev, "gate");
	if (PTR_ERR(data->clk) == -ENOENT) {
		data->clk = NULL;
	} else if (IS_ERR(data->clk)) {
		dev_err(dev, "failed to get clock!\n");
		return PTR_ERR(data->clk);
	}

	INIT_LIST_HEAD(&data->list);
	spin_lock_init(&data->lock);
	data->dev = dev;
	platform_set_drvdata(pdev, data);

	err = samsung_iommu_init_log(&data->log, SYSMMU_EVENT_MAX);
	if (err) {
		dev_err(dev, "failed to initialize log\n");
		return err;
	}

	pm_runtime_enable(dev);
	ret = sysmmu_get_hw_info(data);
	if (ret) {
		dev_err(dev, "failed to get h/w info\n");
		goto err_get_hw_info;
	}

	ret = sysmmu_parse_dt(data->dev, data);
	if (ret)
		goto err_get_hw_info;

	ret = iommu_device_sysfs_add(&data->iommu, data->dev, NULL, dev_name(dev));
	if (ret) {
		dev_err(dev, "failed to register iommu in sysfs\n");
		goto err_get_hw_info;
	}

	err = iommu_device_register(&data->iommu, &samsung_sysmmu_ops, dev);
	if (err) {
		dev_err(dev, "failed to register iommu\n");
		goto err_iommu_register;
	}

	mutex_lock(&sysmmu_global_mutex);
	if (!sysmmu_global_init_done) {
		err = samsung_sysmmu_init_global();
		if (err) {
			dev_err(dev, "failed to initialize global data\n");
			mutex_unlock(&sysmmu_global_mutex);
			goto err_global_init;
		}
	}
	mutex_unlock(&sysmmu_global_mutex);

	dev_info(dev, "initialized IOMMU. Ver %d.%d.%d, %sgate clock\n",
		 MMU_VERSION_MAJOR(data->version),
		 MMU_VERSION_MINOR(data->version),
		 MMU_VERSION_REVISION(data->version),
		 data->clk ? "" : "no ");
	return 0;

err_global_init:
	iommu_device_unregister(&data->iommu);
err_iommu_register:
	iommu_device_sysfs_remove(&data->iommu);
err_get_hw_info:
	pm_runtime_disable(dev);
	samsung_iommu_deinit_log(&data->log);
	return err;
}

static void samsung_sysmmu_device_shutdown(struct platform_device *pdev)
{
}

static int __maybe_unused samsung_sysmmu_runtime_suspend(struct device *sysmmu)
{
	unsigned long flags;
	struct sysmmu_drvdata *drvdata = dev_get_drvdata(sysmmu);

	spin_lock_irqsave(&drvdata->lock, flags);
	drvdata->rpm_count--;
	if (drvdata->attached_count > 0)
		__sysmmu_disable(drvdata);
	spin_unlock_irqrestore(&drvdata->lock, flags);

	SYSMMU_EVENT_LOG_RANGE(drvdata, SYSMMU_EVENT_POWEROFF, 0,
			       drvdata->rpm_count, drvdata->attached_count);
	return 0;
}

static int __maybe_unused samsung_sysmmu_runtime_resume(struct device *sysmmu)
{
	unsigned long flags;
	struct sysmmu_drvdata *drvdata = dev_get_drvdata(sysmmu);

	spin_lock_irqsave(&drvdata->lock, flags);
	drvdata->rpm_count++;
	if (drvdata->attached_count > 0)
		__sysmmu_enable(drvdata);
	spin_unlock_irqrestore(&drvdata->lock, flags);

	SYSMMU_EVENT_LOG_RANGE(drvdata, SYSMMU_EVENT_POWERON, 0,
			       drvdata->rpm_count, drvdata->attached_count);
	return 0;
}

static int __maybe_unused samsung_sysmmu_suspend(struct device *dev)
{
	dev->power.must_resume = true;

	if (pm_runtime_status_suspended(dev))
		return 0;

	return samsung_sysmmu_runtime_suspend(dev);
}

static int __maybe_unused samsung_sysmmu_resume(struct device *dev)
{
	if (pm_runtime_status_suspended(dev))
		return 0;

	return samsung_sysmmu_runtime_resume(dev);
}

static const struct dev_pm_ops samsung_sysmmu_pm_ops = {
	SET_RUNTIME_PM_OPS(samsung_sysmmu_runtime_suspend,
			   samsung_sysmmu_runtime_resume, NULL)
	SET_LATE_SYSTEM_SLEEP_PM_OPS(samsung_sysmmu_suspend,
				     samsung_sysmmu_resume)
};

static const struct of_device_id sysmmu_of_match[] = {
	{ .compatible = "samsung,sysmmu-v9" },
	{ }
};

static struct platform_driver samsung_sysmmu_driver_v9 = {
	.driver	= {
		.name			= "samsung-sysmmu-v9",
		.of_match_table		= of_match_ptr(sysmmu_of_match),
		.pm			= &samsung_sysmmu_pm_ops,
		.suppress_bind_attrs	= true,
	},
	.probe	= samsung_sysmmu_device_probe,
	.shutdown = samsung_sysmmu_device_shutdown,
};
module_platform_driver(samsung_sysmmu_driver_v9);
MODULE_SOFTDEP("pre: samsung-iommu-group");
MODULE_LICENSE("GPL v2");

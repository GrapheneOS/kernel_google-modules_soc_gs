// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd.
 */

#include <linux/dma-iommu.h>
#include <linux/iommu.h>
#include <linux/module.h>
#include <linux/of_iommu.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include "samsung-iommu-v9.h"

#define REG_MMU_PMMU_INDICATOR			0x2FFC
#define REG_MMU_PMMU_INFO			0x3000
#define REG_MMU_SWALKER_INFO			0x3004

#define SET_PMMU_INDICATOR(val)			((val) & 0xF)
#define MMU_PMMU_INFO_VA_WIDTH(reg)		((reg) & 0x1)
#define MMU_SWALKER_INFO_NUM_PMMU(reg)		((reg) & 0xFFFF)

static struct iommu_ops samsung_sysmmu_ops;
static struct platform_driver samsung_sysmmu_driver_v9;

struct samsung_sysmmu_domain {
	struct iommu_domain domain;
	sysmmu_pte_t *page_table;
	atomic_t *lv2entcnt;
	spinlock_t pgtablelock;	/* spinlock to access pagetable	*/
	bool is_va_36bit;
};

static bool sysmmu_global_init_done;
DEFINE_MUTEX(sysmmu_global_mutex); /* Global driver mutex */
static struct device sync_dev;
static struct kmem_cache *flpt_cache_32bit, *flpt_cache_36bit, *slpt_cache;
static bool exist_36bit_va;

static inline u32 __sysmmu_get_hw_version(struct sysmmu_drvdata *data)
{
	return MMU_VERSION_RAW(readl_relaxed(data->sfrbase + REG_MMU_VERSION));
}

static inline u32 __sysmmu_get_num_pmmu(struct sysmmu_drvdata *data)
{
	return MMU_SWALKER_INFO_NUM_PMMU(readl_relaxed(data->sfrbase + REG_MMU_SWALKER_INFO));
}

static inline u32 __sysmmu_get_va_width(struct sysmmu_drvdata *data)
{
	int i;

	for (i = 0; i < data->num_pmmu; i++) {
		writel_relaxed(SET_PMMU_INDICATOR(i), data->sfrbase + REG_MMU_PMMU_INDICATOR);

		if (MMU_PMMU_INFO_VA_WIDTH(readl_relaxed(data->sfrbase + REG_MMU_PMMU_INFO)))
			return VA_WIDTH_36BIT;
	}

	return VA_WIDTH_32BIT;
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

static struct iommu_domain *samsung_sysmmu_domain_alloc(unsigned int type)
{
	struct samsung_sysmmu_domain *domain;
	struct kmem_cache *flpt_cache;
	size_t num_lv1entries;

	if (type != IOMMU_DOMAIN_UNMANAGED &&
	    type != IOMMU_DOMAIN_DMA &&
	    type != IOMMU_DOMAIN_IDENTITY) {
		pr_err("invalid domain type %u\n", type);
		return NULL;
	}

	domain = kzalloc(sizeof(*domain), GFP_KERNEL);
	if (!domain)
		return NULL;

	flpt_cache = exist_36bit_va ? flpt_cache_36bit : flpt_cache_32bit;
	num_lv1entries = exist_36bit_va ? NUM_LV1ENTRIES_36BIT : NUM_LV1ENTRIES_32BIT;
	domain->is_va_36bit = exist_36bit_va;
	exist_36bit_va = false;

	domain->page_table = (sysmmu_pte_t *)kmem_cache_alloc(flpt_cache, GFP_KERNEL | __GFP_ZERO);
	if (!domain->page_table)
		goto err_pgtable;

	domain->lv2entcnt = kcalloc(num_lv1entries, sizeof(*domain->lv2entcnt), GFP_KERNEL);
	if (!domain->lv2entcnt)
		goto err_counter;

	if (type == IOMMU_DOMAIN_DMA) {
		int ret = iommu_get_dma_cookie(&domain->domain);

		if (ret) {
			pr_err("failed to get dma cookie (%d)\n", ret);
			goto err_get_dma_cookie;
		}
	}

	spin_lock_init(&domain->pgtablelock);

	return &domain->domain;

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
	struct kmem_cache *flpt_cache = domain->is_va_36bit ? flpt_cache_36bit : flpt_cache_32bit;

	iommu_put_dma_cookie(dom);
	kmem_cache_free(flpt_cache, domain->page_table);
	kfree(domain->lv2entcnt);
	kfree(domain);
}

static int samsung_sysmmu_attach_dev(struct iommu_domain *dom, struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);

	if (!fwspec || fwspec->ops != &samsung_sysmmu_ops) {
		dev_err(dev, "failed to attach, IOMMU instance data %s.\n",
			!fwspec ? "is not initialized" : "has different ops");
		return -ENXIO;
	}

	if (!dev_iommu_priv_get(dev)) {
		dev_err(dev, "has no IOMMU\n");
		return -ENODEV;
	}

	return 0;
}

static int samsung_sysmmu_map(struct iommu_domain *dom, unsigned long l_iova, phys_addr_t paddr,
			      size_t size, int prot, gfp_t unused)
{
	return 0;
}

static size_t samsung_sysmmu_unmap(struct iommu_domain *dom, unsigned long l_iova, size_t size,
				   struct iommu_iotlb_gather *gather)
{
	return 0;
}

static void samsung_sysmmu_flush_iotlb_all(struct iommu_domain *dom)
{
}

static void samsung_sysmmu_iotlb_sync(struct iommu_domain *dom, struct iommu_iotlb_gather *gather)
{
}

static phys_addr_t samsung_sysmmu_iova_to_phys(struct iommu_domain *dom, dma_addr_t d_iova)
{
	return 0;
}

void samsung_sysmmu_dump_pagetable(struct device *dev, dma_addr_t iova)
{
}

static struct iommu_device *samsung_sysmmu_probe_device(struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);

	if (!fwspec) {
		dev_dbg(dev, "IOMMU instance data is not initialized\n");
		return ERR_PTR(-ENODEV);
	}

	if (fwspec->ops != &samsung_sysmmu_ops) {
		dev_err(dev, "has different IOMMU ops\n");
		return ERR_PTR(-ENODEV);
	}

	return 0;
}

static void samsung_sysmmu_release_device(struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);

	if (!fwspec || fwspec->ops != &samsung_sysmmu_ops)
		return;

	iommu_fwspec_free(dev);
}

static struct iommu_group *samsung_sysmmu_device_group(struct device *dev)
{
	if (device_iommu_mapped(dev))
		return iommu_group_get(dev);

	return generic_device_group(dev);
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
	struct iommu_fwspec *fwspec;
	unsigned int fwid = 0;
	int ret;

	ret = iommu_fwspec_add_ids(dev, &fwid, 1);
	if (ret) {
		dev_err(dev, "failed to add fwspec ids (err:%d)\n", ret);
		iommu_device_unlink(&data->iommu, dev);
		return ret;
	}

	fwspec = dev_iommu_fwspec_get(dev);
	if (!dev_iommu_priv_get(dev)) {
		client = devres_alloc(samsung_sysmmu_clientdata_release,
				      sizeof(*client), GFP_KERNEL);
		if (!client)
			return -ENOMEM;
		client->dev = dev;
		dev_iommu_priv_set(dev, client);
		devres_add(dev, client);
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

	if (!exist_36bit_va && data->va_width == VA_WIDTH_36BIT)
		exist_36bit_va = true;

	return ret;
}

static struct iommu_ops samsung_sysmmu_ops = {
	.capable		= samsung_sysmmu_capable,
	.domain_alloc		= samsung_sysmmu_domain_alloc,
	.domain_free		= samsung_sysmmu_domain_free,
	.attach_dev		= samsung_sysmmu_attach_dev,
	.map			= samsung_sysmmu_map,
	.unmap			= samsung_sysmmu_unmap,
	.flush_iotlb_all	= samsung_sysmmu_flush_iotlb_all,
	.iotlb_sync		= samsung_sysmmu_iotlb_sync,
	.iova_to_phys		= samsung_sysmmu_iova_to_phys,
	.probe_device		= samsung_sysmmu_probe_device,
	.release_device		= samsung_sysmmu_release_device,
	.device_group		= samsung_sysmmu_device_group,
	.of_xlate		= samsung_sysmmu_of_xlate,
	.pgsize_bitmap		= SECT_SIZE | LPAGE_SIZE | SPAGE_SIZE,
};

static irqreturn_t samsung_sysmmu_irq(int irq, void *dev_id)
{
	struct sysmmu_drvdata *drvdata = dev_id;

	dev_info(drvdata->dev, "irq(%d) happened\n", irq);

	/* TODO: Call the fault handler */

	return IRQ_HANDLED;
}

static int sysmmu_get_hw_info(struct sysmmu_drvdata *data)
{
	data->version = __sysmmu_get_hw_version(data);
	data->num_pmmu = __sysmmu_get_num_pmmu(data);
	data->va_width = __sysmmu_get_va_width(data);

	/* TODO: read more capability in HW */

	return 0;
}

static int samsung_sysmmu_init_global(void)
{
	int ret = 0;

	flpt_cache_32bit = kmem_cache_create("samsung-iommu-32bit_lv1table", LV1TABLE_SIZE_32BIT,
					     LV1TABLE_SIZE_32BIT, 0, NULL);
	if (!flpt_cache_32bit)
		return -ENOMEM;

	flpt_cache_36bit = kmem_cache_create("samsung-iommu-36bit_lv1table", LV1TABLE_SIZE_36BIT,
					     LV1TABLE_SIZE_36BIT, 0, NULL);
	if (!flpt_cache_36bit) {
		ret = -ENOMEM;
		goto err_init_flpt_fail;
	}

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
	kmem_cache_destroy(flpt_cache_36bit);

err_init_flpt_fail:
	kmem_cache_destroy(flpt_cache_32bit);

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

	ret = devm_request_irq(dev, irq, samsung_sysmmu_irq, 0,
			       dev_name(dev), data);
	if (ret) {
		dev_err(dev, "unabled to register handler of irq %d\n", irq);
		return ret;
	}

	data->clk = devm_clk_get(dev, "gate");
	if (PTR_ERR(data->clk) == -ENOENT) {
		dev_info(dev, "no gate clock exists. it's okay.\n");
		data->clk = NULL;
	} else if (IS_ERR(data->clk)) {
		dev_err(dev, "failed to get clock!\n");
		return PTR_ERR(data->clk);
	}

	ret = sysmmu_get_hw_info(data);
	if (ret) {
		dev_err(dev, "failed to get h/w info\n");
		return ret;
	}

	data->dev = dev;
	platform_set_drvdata(pdev, data);

	err = iommu_device_sysfs_add(&data->iommu, data->dev, NULL, dev_name(dev));
	if (err) {
		dev_err(dev, "failed to register iommu in sysfs\n");
		return err;
	}

	iommu_device_set_ops(&data->iommu, &samsung_sysmmu_ops);
	iommu_device_set_fwnode(&data->iommu, dev->fwnode);

	err = iommu_device_register(&data->iommu);
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

	dev_info(dev, "initialized IOMMU. Ver %d.%d.%d\n",
		 MMU_VERSION_MAJOR(data->version),
		 MMU_VERSION_MINOR(data->version),
		 MMU_VERSION_REVISION(data->version));
	return 0;

err_global_init:
	iommu_device_unregister(&data->iommu);
err_iommu_register:
	iommu_device_sysfs_remove(&data->iommu);
	return err;
}

static void samsung_sysmmu_device_shutdown(struct platform_device *pdev)
{
}

static const struct of_device_id sysmmu_of_match[] = {
	{ .compatible = "samsung,sysmmu-v9" },
	{ }
};

static struct platform_driver samsung_sysmmu_driver_v9 = {
	.driver	= {
		.name			= "samsung-sysmmu-v9",
		.of_match_table		= of_match_ptr(sysmmu_of_match),
		.suppress_bind_attrs	= true,
	},
	.probe	= samsung_sysmmu_device_probe,
	.shutdown = samsung_sysmmu_device_shutdown,
};
module_platform_driver(samsung_sysmmu_driver_v9);
MODULE_LICENSE("GPL v2");

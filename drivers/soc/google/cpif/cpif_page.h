/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2021 Samsung Electronics.
 *
 */

#ifndef __CPIF_RX_PAGE_H__
#define __CPIF_RX_PAGE_H__

#include "modem_prj.h"

struct cpif_page {
	struct page	*page;
	bool		usable;
	int		offset;
};

struct cpif_page_pool {
	struct cpif_page	**recycling_page_arr;
	struct cpif_page	*tmp_page;
	u32			rpage_arr_idx;
	u32			rpage_arr_len;
	bool			using_tmp_alloc;
};

void cpif_page_pool_delete(struct cpif_page_pool *pool);
void cpif_page_init_tmp_page(struct cpif_page_pool *pool);
struct cpif_page_pool *cpif_page_pool_create(u64 num_page);
void *cpif_cur_page_base(struct cpif_page_pool *pool, bool used_tmp_alloc);
void *cpif_page_alloc(struct cpif_page_pool *pool, u64 alloc_size, bool *used_tmp_alloc);
#endif /* __CPIF_RX_PAGE_H__ */
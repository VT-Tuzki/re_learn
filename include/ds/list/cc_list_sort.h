#ifndef __CC_LIST_SORT_H
#define __CC_LIST_SORT_H

#include "core/cc_common.h"
#include "ds/list/cc_list.h"


int cc_list_sort_bubble(cc_list_t *self, cc_cmp_fn_t cmp);
int _cc_list_sort_bubble(cc_list_t *self, cc_cmp_fn_t cmp, cc_size_t optimize);

int cc_list_sort_quick(cc_list_t *self, cc_cmp_fn_t cmp);
int cc_list_sort_selection(cc_list_t *self, cc_cmp_fn_t cmp);

int cc_list_sort_merge(cc_list_t *self, cc_cmp_fn_t cmp);
int _cc_list_sort_merge(cc_list_t *self, cc_cmp_fn_t cmp, cc_size_t optimize);


int cc_list_sort_check(cc_list_t *self, cc_cmp_fn_t cmp, cc_size_t *dir);
#endif
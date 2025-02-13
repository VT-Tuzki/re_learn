#ifndef __CC_ARRAY_SORT_H
#define __CC_ARRAY_SORT_H

#include "cc_common.h"
#include "cc_array.h"

int cc_array_sort_bubble(struct cc_array *self, cc_cmp_fn_t cmp);
int cc_array_sort_quick(struct cc_array *self, cc_cmp_fn_t cmp);
int cc_array_sort_selection(struct cc_array *self, cc_cmp_fn_t cmp);

int cc_array_sort_merge(struct cc_array *self, cc_cmp_fn_t cmp);

#endif
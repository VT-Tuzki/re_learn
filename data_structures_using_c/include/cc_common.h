#ifndef CC_COMMON_H
#define CC_COMMON_H

typedef int (*cc_cmp_fn_t)(void *left, void *right);

int cc_default_cmp_fn(void *left, void *right);





#endif
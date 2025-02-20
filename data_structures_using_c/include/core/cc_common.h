#ifndef __CC_COMMON_H_
#define __CC_COMMON_H_

#include "core/cc_stdint.h"

#ifndef NOTUSED
#define NOTUSED(x) (void)(x);
#endif







typedef enum {
    ERR_CC_COMMON_NOT_DEFINE = -2,
    ERR_CC_COMMON_FAIL = -1,
    ERR_CC_COMMON_OK = 0,
    ERR_CC_COMMON_INVALID_ARG,
    ERR_CC_COMMON_MEM_ERR,


    ERR_CC_STACK_BEGIN = 0xD0,
} cc_common_err_e;

typedef enum {
    ERR_CC_COMMON_H = 0xA0,
    ERR_CC_ARRAY_H = 0xA1,
    ERR_CC_ARRAY_SORT_H = 0xA2,
    ERR_CC_ARRAY_STACK_H = 0xA3,

    ERR_CC_STACK_H = 0xB0,
    ERR_CC_STACK_SORT_H = 0xB1,
    ERR_CC_LIST_H = 0xC0,
    ERR_CC_LIST_STACK_H = 0xC1,
    ERR_CC_ITER_H = 0xD0,
} cc_element_err_e;

#define ERR_CC_CONNECT(element, error) (element | error)

int cc_default_cmp_fn(void *left, void *right);

typedef int (*cc_cmp_fn_t)(void *left, void *right);
typedef int (*cc_copy_data_fn_t)(void *list_node, void *new_data_ptr);
typedef int (*cc_delete_fn_t)(void *obj);
typedef int (*cc_check_fn_t)(void *obj);
typedef int (*cc_debug_print_fn_t)(void *value);

static inline int try_reset_double_p(void *ptr) {
    if (ptr == NULL)
        return ERR_CC_COMMON_INVALID_ARG;

    *(void **)ptr = NULL;
    return 0;
}

#endif
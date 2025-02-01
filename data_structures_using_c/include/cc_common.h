#ifndef __CC_COMMON_H_
#define __CC_COMMON_H_

typedef int (*cc_cmp_fn_t)(void *left, void *right);

int cc_default_cmp_fn(void *left, void *right);


typedef int (*cc_delete_fn_t)(void *obj);
typedef int (*cc_debug_print_fn_t)(void *value);

typedef enum {
    ERR_CC_COMMON_OK = 0,
    ERR_CC_COMMON_INVALID_ARG,
    ERR_CC_COMMON_MEM_ERR,
} cc_common_err_e;

typedef enum {
    ERR_CC_COMMON_H = 0xA0,
    ERR_CC_ARRAY_H = 0xA1,
    ERR_CC_ARRAY_SORT_H = 0xA2,
    ERR_CC_ARRAY_STACK_H = 0xA3,

    ERR_CC_STACK_H = 0xB0,
    ERR_CC_STACK_SORT_H = 0xB1,
    ERR_CC_LIST_H = 0xC0,
} cc_element_err_e;

#define ERR_CC_CONNECT(element, error) (element | error)

#endif
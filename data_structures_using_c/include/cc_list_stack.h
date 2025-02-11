#ifndef __CC_LIST_STACK_H__
#define __CC_LIST_STACK_H__

#include "cc_common.h"
#include "cc_stack.h"
#include "cc_list.h"

typedef enum {
    ERR_CC_LIST_STACK_UNDEFINE = ERR_CC_LIST_STACK_BEGIN,
    ERR_CC_LIST_STACK_EMPTY,
    ERR_CC_LIST_STACK_FULL,
} cc_list_stack_err;



struct cc_list_stack {
    cc_stack_i_t *interface;
    cc_list_t *list;
};
typedef struct cc_list_stack cc_list_stack_t;

typedef enum {
    ERR_CC_STACK_UNDEFINE = ERR_CC_STACK_BEGIN,
    ERR_CC_STACK_EMPTY,
    ERR_CC_STACK_FULL,
} cc_stack_err;

int cc_list_stack_new(cc_list_stack_t **self);
int cc_list_stack_delete(cc_list_stack_t *self, cc_delete_fn_t remove_fn);

int cc_list_stack_push(cc_list_stack_t *self, void *data);
int cc_list_stack_pop(cc_list_stack_t *self, void **result);
int cc_list_stack_peek(cc_list_stack_t *self, void **result);

cc_size_t cc_array_stack_elem_nums(cc_list_stack_t *self, cc_size_t *elem_nums);


#endif
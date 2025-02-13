#ifndef __CC_ARRAY_STACK_H
#define __CC_ARRAY_STACK_H

#include "cc_array.h"
#include "cc_common.h"
#include "cc_stdint.h"
#include "cc_stack.h"
struct cc_array_stack {
    cc_stack_i_t *interface;
    cc_array_t *data;
    cc_size_t top;
};
typedef struct cc_array_stack cc_array_stack_t;

int cc_array_stack_new(cc_array_stack_t **self, cc_size_t elem_nums, cc_size_t elem_size, cc_delete_fn_t remove_fn);
int cc_array_stack_init(cc_array_stack_t *self, struct cc_array *data);
int cc_array_stack_delete(cc_array_stack_t *self);

#define CC_ARRAY_STACK_STATIC_DECLARE(name, elem_nums, elem_size, remove_fn) \
    CC_ARRAY_STATIC_DECLARE(name##_array, elem_num, elem_size, remove_fn); \
    cc_array_stack_t name = {\
        .interface = &cc_array_stack_interface, \
        .data = name##array, \
        .top = 0 \
    }

int cc_array_stack_push(cc_array_stack_t *self, void *item);
int cc_array_stack_pop(cc_array_stack_t *self, void *item);
int cc_array_stack_peek(cc_array_stack_t *self, void *item);

int cc_array_stack_elem_nums(cc_array_stack_t *self, cc_size_t *elem_nums);
int cc_array_stack_space(cc_array_stack_t *self, cc_size_t *space);


#endif
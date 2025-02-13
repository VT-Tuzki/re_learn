#ifndef __CC_ARRAY_STACK_H
#define __CC_ARRAY_STACK_H

#include "cc_array.h"
#include "cc_common.h"
#include "cc_stdint.h"

struct cc_array_stack {
    struct cc_stack_i *interface;
    struct cc_array *data;
    cc_size_t top;
};

int cc_array_stack_new(struct cc_array_stack **self, cc_size_t elem_nums, cc_size_t elem_size, cc_delete_fn_t remove_fn);
int cc_array_stack_init(struct cc_array_stack *self, struct cc_array *data);
int cc_array_stack_delete(struct cc_array_stack *self);

int cc_array_stack_push(struct cc_array_stack *self, void *item);
int cc_array_stack_pop(struct cc_array_stack *self, void *item);
int cc_array_stack_peek(struct cc_array_stack *self, void *item);

int cc_array_stack_elem_nums(struct cc_array_stack *self, cc_size_t *elem_nums);
int cc_array_stack_space(struct cc_array_stack *self, cc_size_t *space);


#endif
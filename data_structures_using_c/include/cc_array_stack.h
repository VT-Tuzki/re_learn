#ifndef __CC_ARRAY_STACK_H
#define __CC_ARRAY_STACK_H

#include "cc_array.h"
#include "cc_common.h"
#include <stddef.h>

struct cc_array_stack {
    struct cc_stack_i *interface;
    struct cc_array *data;
    size_t top;
};

int cc_array_stack_new(struct cc_array_stack **self, size_t elem_nums, size_t elem_size);
int cc_array_stack_delete(struct cc_array_stack *self, cc_delete_fn_t remove_fn);

int cc_array_stack_init(struct cc_array_stack *self, struct cc_array *data);

int cc_array_stack_push(struct cc_array_stack *self, void *item);
int cc_array_stack_pop(struct cc_array_stack *self, void *item);
int cc_array_stack_peek(struct cc_array_stack *self, void *item);

int cc_array_stack_elem_nums(struct cc_array_stack *self, size_t *elem_nums);
int cc_array_stack_space(struct cc_array_stack *self, size_t *space);


#define CC_ARRAY_STACK_cc_array_stack_new_MASK                  0x01U
#define CC_ARRAY_STACK_cc_array_stack_delete_new_MASK           0x02U
#define CC_ARRAY_STACK_cc_array_stack_init_new_MASK             0x03U
#define CC_ARRAY_STACK_cc_array_stack_push_new_MASK             0x04U
#define CC_ARRAY_STACK_cc_array_stack_pop_new_MASK              0x05U
#define CC_ARRAY_STACK_cc_array_stack_peek_new_MASK             0x06U
#define CC_ARRAY_STACK_cc_array_stack_elem_nums_new_MASK        0x07U
#define CC_ARRAY_STACK_cc_array_stack_space_new_MASK            0x08U





#endif
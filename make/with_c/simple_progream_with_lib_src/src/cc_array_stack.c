#include <stdlib.h>
#include <string.h>

#include "cc_common.h"
#include "cc_stack.h"
#include "cc_array.h"
#include "cc_array_stack.h"

struct cc_stack_i cc_array_stack_interface = {
    .push = (cc_stack_push_fn_t)cc_array_stack_push,
    .pop = (cc_stack_pop_fn_t)cc_array_stack_pop,
    .peek = (cc_stack_peek_fn_t)cc_array_stack_peek,
};


int cc_array_stack_push(struct cc_array_stack *self, void *item) {
    if(cc_array_set(self->data, self->top + 1, item)) {
        return 1;
    }
    self->top++;
    return 0;
}

int cc_array_stack_pop(struct cc_array_stack *self, void *item) {
    if(self->top == 0) {
        return 1;
    }

    self->top--;
    cc_array_get_unsafe(self->data, self->top, item);
    return 0;
}

int cc_array_stack_peek(struct cc_array_stack *self, void *item) {
    if(self->top == 0) {
        return 1;
    }

    cc_array_get_unsafe(self->data, self->top, item);
    return 0;
}

int cc_array_stack_elem_nums(struct cc_array_stack *self, size_t *elem_nums) {
   *elem_nums = self->top;
    return 0;
}

int cc_array_stack_space(struct cc_array_stack *self, size_t *space) {
    *space = self->data->elem_nums - self->top;
    return 0;
}


int cc_array_stack_init(struct cc_array_stack *self, struct cc_array *data) {
    self->interface = &cc_array_stack_interface;
    self->data = data;
    self->top = 0;
    return 0;
}



int cc_array_stack_new(struct cc_array_stack **self, size_t elem_nums, size_t elem_size) {
    struct cc_array_stack *tmp;
    struct cc_array *array;
    tmp = malloc(sizeof( *tmp));

    if(tmp == NULL) {
        goto fail1;
    }

    if(cc_array_new(&array, elem_nums, elem_size)) {
        goto fail2;
    }

    if(cc_array_stack_init(tmp, *array)) {
        goto fail3;
    }

    *self = tmp;
    return 0;

fail3:
    cc_array_delete(array);
fail2:
    free(tmp);
fail1:
    return 1;
}

int cc_array_stack_delete(struct cc_array_stack *self) {
    if(cc_array_delete(self->data)) {
        return 1;
    }
    free(self);
    return 0;
}
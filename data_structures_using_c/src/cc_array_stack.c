#include "core/cc_mem.h"

#include "core/cc_common.h"
#include "ds/stack/cc_stack.h"
#include "ds/array/cc_array.h"
#include "cc_array_stack.h"

struct cc_stack_i cc_array_stack_interface = {
    .push = (cc_stack_push_fn_t)cc_array_stack_push,
    .pop = (cc_stack_pop_fn_t)cc_array_stack_pop,
    .peek = (cc_stack_peek_fn_t)cc_array_stack_peek,
};


int cc_array_stack_push(cc_array_stack_t *self, void *item) {
    if(cc_array_set(self->data, self->top, item)) {
        return 1;
    }
    self->top++;
    return 0;
}

int cc_array_stack_pop(cc_array_stack_t *self, void *item) {
    if(self->top == 0) {
        return 1;
    }

    self->top--;
    cc_array_get_unsafe(self->data, self->top, item);
    return 0;
}

int cc_array_stack_peek(cc_array_stack_t *self, void *item) {
    if(self->top == 0) {
        return 1;
    }

    cc_array_get_unsafe(self->data, self->top - 1, item);
    return 0;
}

int cc_array_stack_elem_nums(cc_array_stack_t *self, cc_size_t *elem_nums) {
   *elem_nums = self->top;
    return 0;
}

int cc_array_stack_space(cc_array_stack_t *self, cc_size_t *space) {
    *space = self->data->elem_nums - self->top;
    return 0;
}


int cc_array_stack_init(cc_array_stack_t *self, struct cc_array *data) {
    self->interface = &cc_array_stack_interface;
    self->data = data;
    self->top = 0;
    return 0;
}



int cc_array_stack_new(cc_array_stack_t **self, cc_size_t elem_nums, cc_size_t elem_size, cc_delete_fn_t remove_fn) {
    cc_array_stack_t *tmp;
    struct cc_array *array;
    tmp = cc_malloc(sizeof( *tmp));

    if(tmp == NULL) {
        goto fail1;
    }

    if(cc_array_new(&array, elem_nums, elem_size, remove_fn)) {
        goto fail2;
    }

    if(cc_array_stack_init(tmp, array)) {
        goto fail3;
    }

    *self = tmp;
    return 0;

fail3:
    cc_array_delete(array);
fail2:
    cc_free(tmp);
fail1:
    return 1;
}

int cc_array_stack_delete(cc_array_stack_t *self) {
    if(cc_array_delete(self->data)) {
        return 1;
    }
    cc_free(self);
    return 0;
}
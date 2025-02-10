#include "cc_array_queue.h"
#include "cc_array.h"
#include <stdlib.h>

cc_queue_i_t cc_array_queue_interface = {
    .enqueue = (cc_queue_enqueue_fn_t) cc_array_queue_enqueue,
    .dequeue = (cc_queue_dequeue_fn_t) cc_array_queue_dequeue,
    .peek = (cc_queue_peek_fn_t) cc_array_queue_peek,
    .is_empty = (cc_queue_is_empty_fn_t) cc_array_queue_is_empty,
    .is_full = (cc_queue_is_full_fn_t) cc_array_queue_is_full,
    .size = (cc_queue_size_fn_t) cc_array_queue_size,
};

int cc_array_queue_new(cc_array_queue_t **self, size_t elem_nums, size_t elem_size)
{
    cc_array_queue_t *temp_array_queue;
    cc_array_t *temp_array;
    int res = ERR_CC_COMMON_OK;
    temp_array_queue = malloc(sizeof(*temp_array_queue));
    if(temp_array_queue == NULL) {
        res = ERR_CC_COMMON_MEM_ERR;
        goto fail1;
    }

    res = cc_array_new(&temp_array, elem_nums, elem_size);
    if(res != ERR_CC_COMMON_OK) {
        goto fail2;
    }
    res = cc_array_queue_init(temp_array_queue, temp_array);
    if(res != ERR_CC_COMMON_OK) {
        goto fail3;
    }

    *self = temp_array_queue;
    return ERR_CC_COMMON_OK;
fail3:
    free(temp_array);
fail2:
    free(temp_array_queue);
fail1:
    return res;
}
int cc_array_queue_delete(cc_array_queue_t *self, cc_delete_fn_t remove_fn)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) return ERR_CC_COMMON_INVALID_ARG;

    res = cc_array_delete(self->array, remove_fn);
    if(res != ERR_CC_COMMON_OK) {
        goto fail1;
    }
    free(self);

    return ERR_CC_COMMON_OK;
fail1:
    return res;
}
int cc_array_queue_init(cc_array_queue_t *self, cc_array_t *data)
{
    self->interface = &cc_array_queue_interface;
    self->array = data;
    self->now_size = 0;
    self->now_front = 0;
    self->now_rear = 0;
    return ERR_CC_COMMON_OK;
}

int cc_array_queue_enqueue(cc_array_queue_t *self, void *data)
{
    int res = ERR_CC_COMMON_OK;

    if(self->now_size == 0) {
        return ERR_CC_QUEUE_EMPTY;
    }

    res = cc_array_set(self->array, self->now_rear, data);
    if(res != ERR_CC_COMMON_OK) return res;
    self->now_rear = (self->now_rear + 1) % self->array->elem_nums;
    self->now_size++;

    return res;
}
int cc_array_queue_dequeue(cc_array_queue_t *self, void **result)
{
    int res = ERR_CC_COMMON_OK;

    if(self->now_size == 0) {
        return ERR_CC_QUEUE_EMPTY;
    }

    res = cc_array_get(self->array, self->now_front, result);
    if(res != ERR_CC_COMMON_OK) return res;
    self->now_front = (self->now_front + 1) % self->array->elem_nums;
    self->now_size--;

    return res;
}
int cc_array_queue_peek(cc_array_queue_t *self, void **result)
{
    int res = ERR_CC_COMMON_OK;

    if(self->now_size == 0) {
        return ERR_CC_QUEUE_EMPTY;
    }

    res = cc_array_get(self->array, self->now_front, result);

    return res;
}

int cc_array_queue_is_empty(cc_array_queue_t *self)
{
    if(self->now_size == 0) return ERR_CC_QUEUE_EMPTY;
    else return ERR_CC_COMMON_OK;
}
int cc_array_queue_is_full(cc_array_queue_t *self)
{
    if(self->now_size >= self->array->elem_nums) {
        return ERR_CC_QUEUE_IS_FULL;
    }
    return ERR_CC_COMMON_OK;
}
size_t cc_array_queue_size(cc_array_queue_t *self)
{
    return self->now_size;
}


#include "cc_array_ring_queue.h"
#include "ds/array/cc_array.h"
#include "core/cc_mem.h"

cc_ring_queue_i_t cc_array_ring_queue_interface = {
    .enqueue = (cc_ring_queue_enqueue_fn_t) cc_array_ring_queue_enqueue,
    .dequeue = (cc_ring_queue_dequeue_fn_t) cc_array_ring_queue_dequeue,
    .peek = (cc_ring_queue_peek_fn_t) cc_array_ring_queue_peek,
    .is_empty = (cc_ring_queue_is_empty_fn_t) cc_array_ring_queue_is_empty,
    .is_full = (cc_ring_queue_is_full_fn_t) cc_array_ring_queue_is_full,
    .size = (cc_ring_queue_size_fn_t) cc_array_ring_queue_size,
    .capacity = (cc_ring_queue_capacity_fn_t) cc_array_ring_queue_capacity,
};

int cc_array_ring_queue_new(cc_array_ring_queue_t **self, cc_size_t elem_nums, cc_size_t elem_size, cc_delete_fn_t remove_fn)
{
    cc_array_ring_queue_t *temp_array_ring_queue;
    cc_array_t *temp_array;
    int res = ERR_CC_COMMON_OK;
    temp_array_ring_queue = cc_malloc(sizeof(*temp_array_ring_queue));
    if(temp_array_ring_queue == NULL) {
        res = ERR_CC_COMMON_MEM_ERR;
        goto fail1;
    }

    res = cc_array_new(&temp_array, elem_nums, elem_size, remove_fn);
    if(res != ERR_CC_COMMON_OK) {
        goto fail2;
    }
    res = cc_array_ring_queue_init(temp_array_ring_queue, temp_array);
    if(res != ERR_CC_COMMON_OK) {
        goto fail3;
    }

    *self = temp_array_ring_queue;
    return ERR_CC_COMMON_OK;
fail3:
    cc_free(temp_array);
fail2:
    cc_free(temp_array_ring_queue);
fail1:
    return res;
}

int cc_array_ring_queue_delete(cc_array_ring_queue_t *self)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) return ERR_CC_COMMON_INVALID_ARG;

    res = cc_array_delete(self->array);
    if(res != ERR_CC_COMMON_OK) {
        goto fail1;
    }
    cc_free(self);

    return ERR_CC_COMMON_OK;
fail1:
    return res;
}

int cc_array_ring_queue_init(cc_array_ring_queue_t *self, cc_array_t *data)
{
    self->interface = &cc_array_ring_queue_interface;
    self->array = data;
    self->write_idx = 0;
    self->read_idx = 0;
    return ERR_CC_COMMON_OK;
}

int cc_array_ring_queue_enqueue(cc_array_ring_queue_t *self, void *data)
{
    int res = ERR_CC_COMMON_OK;

    if(cc_array_ring_queue_is_full(self) == ERR_CC_QUEUE_IS_FULL) {
        return ERR_CC_QUEUE_IS_FULL;
    }

    res = cc_array_set(self->array, self->write_idx, data);
    if(res != ERR_CC_COMMON_OK) return res;
    self->write_idx = (self->write_idx + 1) % self->array->elem_nums;

    return res;
}

int cc_array_ring_queue_dequeue(cc_array_ring_queue_t *self, void **result)
{
    int res = ERR_CC_COMMON_OK;

    if(cc_array_ring_queue_is_empty(self) == ERR_CC_QUEUE_EMPTY) {
        return ERR_CC_QUEUE_EMPTY;
    }
    res = cc_array_get(self->array, self->read_idx, result);
    if(res != ERR_CC_COMMON_OK) return res;
    self->read_idx = (self->read_idx + 1) % self->array->elem_nums;

    return res;
}

int cc_array_ring_queue_peek(cc_array_ring_queue_t *self, void **result)
{
    int res = ERR_CC_COMMON_OK;

    if(cc_array_ring_queue_is_empty(self) == ERR_CC_QUEUE_EMPTY) {
        return ERR_CC_QUEUE_EMPTY;
    }

    res = cc_array_get(self->array, self->read_idx, result);

    return res;
}



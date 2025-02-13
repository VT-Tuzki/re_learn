#ifndef __CC_ARRAY_QUEUE_H__
#define __CC_ARRAY_QUEUE_H__

#include "ds/array/cc_array.h"
#include "cc_ring_queue.h"
struct cc_array_ring_queue {
    cc_ring_queue_i_t *interface;
    cc_array_t *array;
    cc_size_t write_idx;
    cc_size_t read_idx;
};
typedef struct cc_array_ring_queue cc_array_ring_queue_t;



int cc_array_ring_queue_new(cc_array_ring_queue_t **self, cc_size_t elem_nums, cc_size_t elem_size, cc_delete_fn_t remove_fn);
int cc_array_ring_queue_init(cc_array_ring_queue_t *self, cc_array_t *data);
int cc_array_ring_queue_delete(cc_array_ring_queue_t *self);

int cc_array_ring_queue_enqueue(cc_array_ring_queue_t *self, void *data);
int cc_array_ring_queue_dequeue(cc_array_ring_queue_t *self, void **result);
int cc_array_ring_queue_peek(cc_array_ring_queue_t *self, void **result);

static inline int cc_array_ring_queue_is_empty(cc_array_ring_queue_t *self)
{
    if(self->write_idx == self->read_idx) return ERR_CC_QUEUE_EMPTY;
    else return ERR_CC_COMMON_OK;
};

static inline int cc_array_ring_queue_is_full(cc_array_ring_queue_t *self)
{
    if ((self->write_idx + 1) % self->array->elem_nums == self->read_idx) {
        return ERR_CC_QUEUE_IS_FULL;
    }
    return ERR_CC_COMMON_OK;
};

static inline cc_size_t cc_array_ring_queue_size(cc_array_ring_queue_t *self)
{
    if(self->write_idx < self->read_idx) {
        return self->array->elem_size - self->read_idx + self->write_idx;
    }
    else {
        return self->write_idx - self->read_idx;
    }
};

static inline cc_size_t cc_array_ring_queue_capacity(cc_array_ring_queue_t *self)
{
    return self->array->elem_nums - 1;
};

#define CC_ARRAY_RING_QUEUE_STATIC_DECLARE(name, elem_num, elem_size, remove_fn) \
    CC_ARRAY_STATIC_DECLARE(name##_array, elem_num, elem_size, remove_fn); \
    cc_array_ring_queue_t name = { \
        .interface = &cc_array_ring_queue_interface, \
        .array = &name##_array, \
        .write_idx = 0, \
        .read_idx = 0, \
        .remove_fn = (cc_delete_fn_t) (remove_fn), \
    }

#endif
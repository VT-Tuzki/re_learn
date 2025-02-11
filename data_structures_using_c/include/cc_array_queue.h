#ifndef __CC_ARRAY_QUEUE_H__
#define __CC_ARRAY_QUEUE_H__

#include "cc_array.h"
#include "cc_queue.h"

struct cc_array_queue {
    cc_queue_i_t *interface;
    cc_array_t *array;
    cc_size_t now_size;
    cc_size_t now_front;
    cc_size_t now_rear;
};
typedef struct cc_array_queue cc_array_queue_t;



int cc_array_queue_new(cc_array_queue_t **self, cc_size_t elem_nums, cc_size_t elem_size);
int cc_array_queue_delete(cc_array_queue_t *self, cc_delete_fn_t remove_fn);
int cc_array_queue_init(cc_array_queue_t *self, cc_array_t *data);
int cc_array_queue_enqueue(cc_array_queue_t *self, void *data);
int cc_array_queue_dequeue(cc_array_queue_t *self, void **result);
int cc_array_queue_peek(cc_array_queue_t *self, void **result);

int cc_array_queue_is_empty(cc_array_queue_t *self);
int cc_array_queue_is_full(cc_array_queue_t *self);
cc_size_t cc_array_queue_size(cc_array_queue_t *self);



#endif
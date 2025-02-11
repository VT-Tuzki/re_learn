#ifndef __CC_LIST_QUEUE_H__
#define __CC_LIST_QUEUE_H__

#include "cc_list.h"
#include "cc_queue.h"

struct cc_list_queue {
    cc_queue_i_t *interface;
    cc_list_t *list;
};
typedef struct cc_list_queue cc_list_queue_t;

int cc_list_queue_new(cc_list_queue_t **self);
int cc_list_queue_delete(cc_list_queue_t *self, cc_delete_fn_t remove_fn);
int cc_list_queue_enqueue(cc_list_queue_t *self, void *data);
int cc_list_queue_dequeue(cc_list_queue_t *self, void **result);
int cc_list_queue_peek(cc_list_queue_t *self, void **result);

int cc_list_queue_is_empty(cc_list_queue_t *self);
int cc_list_queue_is_full(cc_list_queue_t *self);
cc_size_t cc_list_queue_size(cc_list_queue_t *self);

#endif
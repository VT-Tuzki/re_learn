#ifndef __CC_QUEUE_H__
#define __CC_QUEUE_H__

#include "../../core/cc_common.h"
#include "../../core/cc_stdint.h"

typedef int (*cc_queue_enqueue_fn_t) (void *self, void *data);
typedef int (*cc_queue_dequeue_fn_t) (void *self, void **result);
typedef int (*cc_queue_peek_fn_t) (void *self, void **result);
typedef cc_size_t (*cc_queue_size_fn_t) (void *self);
typedef int (*cc_queue_is_empty_fn_t) (void *self);
typedef int (*cc_queue_is_full_fn_t) (void *self);

struct cc_queue_i {
    cc_queue_enqueue_fn_t enqueue;
    cc_queue_dequeue_fn_t dequeue;
    cc_queue_peek_fn_t peek;
    cc_queue_is_empty_fn_t is_empty;
    cc_queue_is_full_fn_t is_full;
    cc_queue_size_fn_t size;
};

typedef struct cc_queue_i cc_queue_i_t;

typedef enum {
    ERR_CC_QUEUE_OK = ERR_CC_COMMON_OK,
    ERR_CC_QUEUE_INVALID_ARG = ERR_CC_COMMON_INVALID_ARG,
    ERR_CC_QUEUE_MEM_ERR = ERR_CC_COMMON_MEM_ERR,
    ERR_CC_QUEUE_EMPTY,
    ERR_CC_QUEUE_IS_FULL,


} cc_queue_err;

static inline int cc_queue_enqueue (void *self, void *data) {
    return (*(cc_queue_i_t **)self)->enqueue(self, data);
}

static inline int cc_queue_dequeue (void *self, void **result) {
    return (*(cc_queue_i_t **)self)->dequeue(self, result);
}

static inline int cc_queue_peek (void *self, void **result) {
    return (*(cc_queue_i_t **)self)->peek(self, result);
}

static inline int cc_queue_is_empty (void *self) {
    return (*(cc_queue_i_t **)self)->is_empty(self);
}

static inline int cc_queue_is_full (void *self) {
    return (*(cc_queue_i_t **)self)->is_full(self);
}

static inline cc_size_t cc_queue_size (void *self) {
    return (*(cc_queue_i_t **)self)->size(self);
}

#endif
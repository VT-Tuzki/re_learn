#ifndef __CC_LIST_POOL_H__
#define __CC_LIST_POOL_H__

#include "cc_list.h"
#include "cc_pool.h"

typedef struct cc_list_node_pool {
    cc_pool_t pool;
} cc_list_node_pool_t;

typedef struct cc_list_with_pool {
    cc_list_t list;
    cc_list_node_pool_t *pool;
} cc_list_with_pool_t;

int cc_list_node_pool_init(cc_list_node_pool_t *node_pool, cc_size_t capacity);

#define CC_LIST_NODE_POOL_STATIC_DECLARE(name, capacity) \
    CC_POOL_STATIC_DECLARE(name##_pool, cc_list_node_t, capacity, NULL);    \
    cc_list_node_pool_t (name) = {0};

#define CC_LIST_NODE_POOL_CONNECT(name) \
    do { \
        (name).pool = name##_pool; \
    } while(0)

int cc_list_node_pool_static_init(cc_list_node_pool_t *node_pool);
int cc_list_node_pool_acquire(cc_list_node_pool_t *node_pool, cc_list_node_t **node);
int cc_list_node_pool_release(cc_list_node_pool_t *node_pool, cc_list_node_t *node);
int cc_list_node_pool_destroy(cc_list_node_pool_t *node_pool);

int cc_list_node_insert_before_with_pool(cc_list_node_pool_t *pool, cc_list_node_t *self, void *data);
int cc_list_node_insert_after_with_pool(cc_list_node_pool_t *pool, cc_list_node_t *self, void *data);
int cc_list_node_remove_before_with_pool(cc_list_node_pool_t *pool, cc_list_node_t *self, void **result);
int cc_list_node_remove_after_with_pool(cc_list_node_pool_t *pool, cc_list_node_t *self, void **result);

int cc_list_init_with_pool(cc_list_with_pool_t *list_with_pool, cc_list_node_pool_t *pool, cc_delete_fn_t remove_fn);
int cc_list_new_with_pool(cc_list_with_pool_t **list_with_pool, cc_list_node_pool_t *pool, cc_delete_fn_t remove_fn);
int cc_list_destroy_with_pool(cc_list_with_pool_t *list_with_pool);

int cc_list_insert_head_with_pool(cc_list_with_pool_t *list_with_pool, void *data);
int cc_list_insert_tail_with_pool(cc_list_with_pool_t *list_with_pool, void *data);
int cc_list_remove_head_with_pool(cc_list_with_pool_t *list_with_pool, void **data);
int cc_list_remove_tail_with_pool(cc_list_with_pool_t *list_with_pool, void **data);
int cc_list_remove_node_with_pool(cc_list_with_pool_t *list_with_pool, cc_list_node_t *node, void **data);

#endif // __CC_LIST_POOL_H__

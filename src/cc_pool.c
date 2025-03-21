
#include "cc_common.h"
#include "cc_pool.h"
#include <stdint.h>



int cc_pool_init(cc_pool_t *pool,  cc_size_t user_elem_size, cc_size_t capacity, cc_delete_fn_t remove_fn)
{
    int res = ERR_CC_COMMON_OK;
    if(capacity == 0) return ERR_CC_COMMON_INVALID_ARG;
    const cc_size_t meta_size = offsetof(pool_node_t, user_data);
    pool->elem_size = meta_size + user_elem_size;
    pool->user_elem_size = user_elem_size;
    pool->capacity = capacity;
    pool->remove_fn = remove_fn;

    res = cc_array_new(&pool->storage, capacity, pool->elem_size, NULL);
    if(res != ERR_CC_COMMON_OK) return res;

    for(cc_size_t i = 0; i < capacity; i++) {
        pool_node_t *node;
        res = cc_array_get_ref(pool->storage, i, (void**)&node);
        if(res != ERR_CC_COMMON_OK) return res;
        node->next_free = (i == capacity-1) ? (pool->capacity+1) : i+1;
        node->is_allocated = 0;
    }
    pool->free_head = 0;
    pool->available_size = pool->capacity;
    pool->init_flag = CC_MAGIC_INIT;
    return res;
}

int cc_pool_static_init(cc_pool_t *pool)
{
    int res = ERR_CC_COMMON_OK;
    if( pool->init_flag == CC_MAGIC_INIT) {
        return ERR_CC_COMMON_FAIL;
    }

    for(cc_size_t i = 0; i < pool->capacity; i++) {
        pool_node_t *node;
        res = cc_array_get_ref(pool->storage, i, (void**)&node);
        if(res != ERR_CC_COMMON_OK) return res;
        node->next_free = (i == pool->capacity - 1) ? (pool->capacity+1) : i+1;
        node->is_allocated = 0;
    }
    pool->free_head = 0;
    pool->available_size = pool->capacity;
    pool->init_flag = CC_MAGIC_INIT;
    return res;
}

int cc_pool_acquire(cc_pool_t *pool, void **ptr)
{
    pool_node_t *node;
    int res = ERR_CC_COMMON_OK;
    *ptr = NULL;
    if(pool->free_head == pool->capacity+1) return ERR_CC_POOL_EMPTY;

    res = cc_array_get_ref(pool->storage, pool->free_head, (void**)&node);
    if(res != ERR_CC_COMMON_OK) return res;

    pool->free_head = node->next_free;
    node->is_allocated = 1;
    pool->available_size--;
    *ptr = node->user_data;

    return res;
}
int cc_pool_acquire_batch(cc_pool_t *pool, void **ptr_array, cc_size_t count)
{
    cc_size_t acuired = 0;
    int res = ERR_CC_COMMON_OK;
    while(pool->free_head != pool->capacity+1 && acuired < count) {
        res = cc_pool_acquire(pool, &ptr_array[acuired++]);
        if(res != ERR_CC_COMMON_OK) return res;
    }
    return res;
}
int cc_pool_release_ptr(cc_pool_t *pool, void *ptr)
{
    if(ptr == NULL) return ERR_CC_COMMON_INVALID_ARG;

    pool_node_t *node = (pool_node_t *)((uintptr_t)ptr - offsetof(pool_node_t, user_data));

    const uintptr_t base = (uintptr_t)pool->storage->data;
    const uintptr_t offset = (uintptr_t)node - base;
    if((offset % (pool->elem_size) != 0) ||
        ((offset / (pool->elem_size)) >= pool->capacity)) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if(pool->remove_fn) {
        pool->remove_fn(ptr);
    }

    //头插法
    node->next_free = pool->free_head;
    node->is_allocated = 0;
    pool->available_size++;
    pool->free_head = offset / pool->elem_size;

    return ERR_CC_COMMON_OK;
}


int cc_pool_destroy(cc_pool_t *pool)
{
    if(pool == NULL) return ERR_CC_COMMON_INVALID_ARG;

    if(pool->storage) {
        int res = cc_array_delete(pool->storage);
        if(res != ERR_CC_COMMON_OK) return res;
        pool->storage = NULL;
    }
    return ERR_CC_COMMON_OK;
}


int cc_pool_get_available_size(cc_pool_t *pool, cc_size_t *available_size)
{
    if(pool == NULL) return ERR_CC_COMMON_INVALID_ARG;

    *available_size = pool->available_size;
    return ERR_CC_COMMON_OK;
}

int cc_pool_get_capacity(cc_pool_t *pool, cc_size_t *capacity)
{
    if(pool == NULL) return ERR_CC_COMMON_INVALID_ARG;

    *capacity = pool->capacity;
    return ERR_CC_COMMON_OK;
}

int cc_pool_is_full(cc_pool_t *pool, uint8_t *is_full)
{
    if(pool == NULL) return ERR_CC_COMMON_INVALID_ARG;

    if(pool->free_head == pool->capacity+1) {
        *is_full = 1;
    } else {
        *is_full = 0;
    }

    return ERR_CC_COMMON_OK;
}
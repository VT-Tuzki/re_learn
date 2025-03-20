#ifndef __CC_POOL_H__
#define __CC_POOL_H__

#include "../array/cc_array.h"
#include "../list/cc_list.h"

/*
TODO:
    中断锁
    原子操作
    rtos互斥锁
    内存对齐/空间优化
    RTOS特定的错误码
    RTOS优先级反转问题
    中断上下文测试
    动态扩容
*/
struct pool_node {
    cc_size_t next_free;
    uint8_t is_allocated;
    uint8_t user_data[];
};
typedef struct pool_node pool_node_t;

struct cc_pool {
    cc_array_t *storage;
    cc_size_t elem_size;
    cc_size_t user_elem_size;
    cc_size_t free_head;
    cc_size_t capacity;
    cc_delete_fn_t remove_fn;
    uint8_t init_flag;
};
typedef struct cc_pool cc_pool_t;
/*
// 动态初始化
cc_pool_init() + cc_pool_destroy()
// 静态初始化
CC_POOL_STATIC_DECLARE() // 无需显式销毁
cc_pool_static_init()

*/
typedef enum {
    ERR_CC_POOL_OK = ERR_CC_COMMON_OK,
    ERR_CC_POOL_INVALID_ARG = ERR_CC_COMMON_INVALID_ARG,
    ERR_CC_POOL_MEM_ERR = ERR_CC_COMMON_MEM_ERR,
    ERR_CC_POOL_EMPTY,
    ERR_CC_POOL_IS_FULL,


} cc_ring_queue_err;
int cc_pool_init(cc_pool_t *pool,  cc_size_t user_elem_size, cc_size_t capacity, cc_delete_fn_t remove_fn);

#define CC_POOL_STATIC_DECLARE(name, set_elem_type, set_capacity,                   \
                                set_remove_fn)                                      \
    enum { offset_userdata##_name = offsetof(pool_node_t, user_data) };             \
    CC_ARRAY_STATIC_DECLARE(name##_storage, set_capacity,                           \
                            (offset_userdata##_name + sizeof(set_elem_type)),       \
                            (set_remove_fn));                                       \
    cc_pool_t name = {.storage = &name##_storage,                                   \
                        .elem_size =                                                \
                            (offset_userdata##_name) + sizeof(set_elem_type),       \
                        .user_elem_size = sizeof(set_elem_type),                    \
                        .capacity = (set_capacity),                                 \
                        .remove_fn = (set_remove_fn),                               \
                        .free_head = (0)};

int cc_pool_static_init(cc_pool_t *pool);

int cc_pool_acquire(cc_pool_t *pool, void **ptr);
int cc_pool_acquire_batch(cc_pool_t *pool, void **ptr_array, cc_size_t count);
int cc_pool_release_ptr(cc_pool_t *pool, void *ptr);
int cc_pool_destroy(cc_pool_t *pool);
#endif //__CC_POOL_H__
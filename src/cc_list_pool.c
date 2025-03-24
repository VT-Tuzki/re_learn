#include <string.h>

#include "cc_stdint.h"
#include "cc_common.h"
#include "cc_mem.h"
#include "cc_dbg.h"

#include "cc_list_pool.h"

int cc_list_node_pool_init(cc_list_node_pool_t *node_pool, cc_size_t capacity)
{
    if (node_pool == NULL || capacity == 0) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    return cc_pool_init(&node_pool->pool, sizeof(cc_list_node_t), capacity, NULL);
}

int cc_list_node_pool_static_init(cc_list_node_pool_t *node_pool)
{
    if (node_pool == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }
    return cc_pool_static_init(&node_pool->pool);
}

int cc_list_node_pool_acquire(cc_list_node_pool_t *node_pool, cc_list_node_t **node)
{
    if (node_pool == NULL || node == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    return cc_pool_acquire(&node_pool->pool, (void**)node);
}

int cc_list_node_pool_release(cc_list_node_pool_t *node_pool, cc_list_node_t *node)
{
    if (node_pool == NULL || node == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    // 清除节点指针，防止悬空引用
    node->next = NULL;
    node->prev = NULL;
    node->data = NULL;

    return cc_pool_release_ptr(&node_pool->pool, node);
}

int cc_list_node_pool_destroy(cc_list_node_pool_t *node_pool)
{
    if (node_pool == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    return cc_pool_destroy(&node_pool->pool);
}

int cc_list_node_insert_before_with_pool(cc_list_node_pool_t *pool, cc_list_node_t *self, void *data)
{
    cc_list_node_t *temp;
    int res;

    if (pool == NULL || self == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    res = cc_list_node_pool_acquire(pool, &temp);
    if (res != ERR_CC_COMMON_OK) {
        return res;
    }

    temp->data = data;
    temp->prev = self->prev;
    temp->next = self;
    self->prev->next = temp;
    self->prev = temp;

    return ERR_CC_COMMON_OK;
}

int cc_list_node_insert_after_with_pool(cc_list_node_pool_t *pool, cc_list_node_t *self, void *data)
{
    cc_list_node_t *temp;
    int res;

    if (pool == NULL || self == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    res = cc_list_node_pool_acquire(pool, &temp);
    if (res != ERR_CC_COMMON_OK) {
        return res;
    }

    temp->data = data;
    temp->next = self->next;
    temp->prev = self;
    self->next->prev = temp;
    self->next = temp;

    return ERR_CC_COMMON_OK;
}

int cc_list_node_remove_before_with_pool(cc_list_node_pool_t *pool, cc_list_node_t *self, void **result)
{
    cc_list_node_t *temp;

    if (pool == NULL || self == NULL || result == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if (self->prev == self) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    temp = self->prev;
    *result = temp->data;

    temp->prev->next = self;
    self->prev = temp->prev;

    return cc_list_node_pool_release(pool, temp);
}

int cc_list_node_remove_after_with_pool(cc_list_node_pool_t *pool, cc_list_node_t *self, void **result)
{
    cc_list_node_t *temp;

    if (pool == NULL || self == NULL || result == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if (self->next == self) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    temp = self->next;
    *result = temp->data;

    temp->next->prev = self;
    self->next = temp->next;

    return cc_list_node_pool_release(pool, temp);
}

int cc_list_init_with_pool(cc_list_with_pool_t *list_with_pool, cc_list_node_pool_t *pool, cc_delete_fn_t remove_fn)
{
    if (list_with_pool == NULL || pool == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    int res = cc_list_init(&list_with_pool->list, remove_fn);
    if (res != ERR_CC_COMMON_OK) {
        return res;
    }

    list_with_pool->pool = pool;
    return ERR_CC_COMMON_OK;
}

int cc_list_new_with_pool(cc_list_with_pool_t **list_with_pool, cc_list_node_pool_t *pool, cc_delete_fn_t remove_fn)
{
    cc_list_with_pool_t *temp;

    if (list_with_pool == NULL || pool == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    temp = (cc_list_with_pool_t *)malloc(sizeof(cc_list_with_pool_t));
    if (temp == NULL) {
        return ERR_CC_COMMON_MEM_ERR;
    }

    int res = cc_list_init_with_pool(temp, pool, remove_fn);
    if (res != ERR_CC_COMMON_OK) {
        free(temp);
        return res;
    }

    *list_with_pool = temp;
    return ERR_CC_COMMON_OK;
}

int cc_list_destroy_with_pool(cc_list_with_pool_t *list_with_pool)
{
    if (list_with_pool == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    cc_list_node_t *current = list_with_pool->list.root.next;

    while (current != &list_with_pool->list.root) {
        cc_list_node_t *next = current->next;

        if (list_with_pool->list.remove_fn) {
            list_with_pool->list.remove_fn(current->data);
        }

        cc_list_node_pool_release(list_with_pool->pool, current);
        current = next;
        list_with_pool->list.root.size--;
    }

    free(list_with_pool);
    return ERR_CC_COMMON_OK;
}

int cc_list_insert_head_with_pool(cc_list_with_pool_t *list_with_pool, void *data)
{
    if (list_with_pool == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    int res = cc_list_node_insert_after_with_pool(list_with_pool->pool, &list_with_pool->list.root, data);

    if (res == ERR_CC_COMMON_OK) {
        list_with_pool->list.root.size++;
    }

    return res;
}

int cc_list_insert_tail_with_pool(cc_list_with_pool_t *list_with_pool, void *data)
{
    if (list_with_pool == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    int res = cc_list_node_insert_before_with_pool(list_with_pool->pool, &list_with_pool->list.root, data);

    if (res == ERR_CC_COMMON_OK) {
        list_with_pool->list.root.size++;
    }

    return res;
}

int cc_list_remove_head_with_pool(cc_list_with_pool_t *list_with_pool, void **data)
{
    if (list_with_pool == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if (cc_list_is_empty(&list_with_pool->list)) {
        return ERR_CC_LIST_EMPTY;
    }

    int res = cc_list_node_remove_after_with_pool(list_with_pool->pool, &list_with_pool->list.root, data);

    if (res == ERR_CC_COMMON_OK) {
        list_with_pool->list.root.size--;
    }

    return res;
}

int cc_list_remove_tail_with_pool(cc_list_with_pool_t *list_with_pool, void **data)
{
    if (list_with_pool == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if (cc_list_is_empty(&list_with_pool->list)) {
        return ERR_CC_LIST_EMPTY;
    }

    int res = cc_list_node_remove_before_with_pool(list_with_pool->pool, &list_with_pool->list.root, data);

    if (res == ERR_CC_COMMON_OK) {
        list_with_pool->list.root.size--;
    }

    return res;
}

int cc_list_remove_node_with_pool(cc_list_with_pool_t *list_with_pool, cc_list_node_t *node, void **data)
{
    if (list_with_pool == NULL || node == NULL) return ERR_CC_LIST_INVALID_ARG;

    // Check if node is within this list
    cc_list_node_t *curr;
    int found = 0;

    cc_list_for_each(curr, &list_with_pool->list.root) {
        if (curr == node) {
            found = 1;
            break;
        }
    }

    if (!found) return ERR_CC_LIST_INVALID_ARG;

    // Save data if requested
    if (data != NULL) {
        *data = node->data;
    }

    // Unlink node from list
    node->prev->next = node->next;
    node->next->prev = node->prev;

    // Update list size
    list_with_pool->list.root.size--;

    // Return node to pool instead of freeing it
    return cc_list_node_pool_release(list_with_pool->pool, node);
}

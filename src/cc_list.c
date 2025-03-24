
#include <string.h>

#include "cc_stdint.h"
#include "cc_common.h"
#include "cc_mem.h"
#include "cc_dbg.h"

#include "cc_list.h"

int cc_list_node_insert_before(cc_list_node_t *self, void *data)
{
    cc_list_node_t *temp;

    temp = (cc_list_node_t *)malloc(sizeof(cc_list_node_t));
    if (temp == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    temp->data = data;
    temp->prev = self->prev;
    temp->next = self;
    self->prev->next = temp;
    self->prev = temp;

    return ERR_CC_COMMON_OK;
}

int cc_list_node_insert_after(cc_list_node_t *self, void *data)
{
    cc_list_node_t *temp;

    temp = (cc_list_node_t *)malloc(sizeof(cc_list_node_t));
    if (temp == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    temp->data = data;
    temp->next = self->next;
    temp->prev = self;
    self->next->prev = temp;
    self->next = temp;

    return ERR_CC_COMMON_OK;
}

int cc_list_node_insert_node(cc_list_node_t *insert_node, cc_list_node_t *prev, cc_list_node_t *next)
{
    int res = ERR_CC_COMMON_OK;
    if(insert_node == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    next->prev = insert_node;
    insert_node->next = next;
    insert_node->prev = prev;
    prev->next = insert_node;
    return res;
}

int cc_list_node_remove_before(cc_list_node_t *self, void **result)
{
    cc_list_node_t *temp;

    if (self->prev == self) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    temp = self->prev;
    *result = temp->data;

    temp->prev->next = self;
    self->prev = temp->prev;


    adapter_free(temp);
    return ERR_CC_COMMON_OK;
}

int cc_list_node_remove_after(cc_list_node_t *self, void **result)
{
    cc_list_node_t *temp;

    if(self->next == self) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    temp = self->next;
    *result = temp->data;


    temp->next->prev = self;
    self->next = temp->next;

    adapter_free(temp);
    return ERR_CC_COMMON_OK;
}

int cc_list_node_remove_node(cc_list_node_t *prev, cc_list_node_t *next)
{
    next->prev = prev;
    prev->next = next;
    return ERR_CC_COMMON_OK;
}

int cc_list_node_delete_node(cc_list_node_t *node)
{
    cc_list_node_remove_node(node->prev, node->next);
    node->next = NULL;
    node->prev = NULL;
    return ERR_CC_COMMON_OK;
}


/*
    list: ...->others_before->current->others_after->....
    cc_list_node_t *temp = current
    cc_list_node_delete_and_next(&temp);
    result: temp = others_after adapter_free(current);
    list: ...->others_before->others_after->....
*/
int cc_list_node_delete_and_next(cc_list_node_t **current, cc_delete_fn_t remove_fn)
{
    cc_list_node_t *temp, *next;
    void *data;
    if(*current == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    temp = *current;
    next = (*current)->next;
    data = (*current)->data;

    if(remove_fn != NULL) {
        remove_fn(data);
    }
    (*current)->data = NULL;

    temp->prev->next = next;
    next->prev = temp->prev;
    adapter_free(temp);

    *current = next;
    return ERR_CC_COMMON_OK;
}


int cc_list_init(cc_list_t *self, cc_delete_fn_t remove_fn)
{
    if(self == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    self->root.prev = &self->root;
    self->root.next = &self->root;
    self->root.size = 0;
    self->remove_fn = remove_fn;
    return ERR_CC_COMMON_OK;
}

int cc_list_new(cc_list_t **self, cc_delete_fn_t remove_fn)
{
    cc_list_t *temp;
    temp = (cc_list_t *)malloc(sizeof(cc_list_t));
    if (temp == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    cc_list_init(temp, remove_fn);

    *self = temp;
    return ERR_CC_COMMON_OK;
}

int cc_list_destroy(cc_list_t *self)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }
    cc_list_node_t *temp;

    temp = self->root.next;

    while(temp != &self->root) {
        res = cc_list_node_delete_and_next(&temp, self->remove_fn);
        if(res != ERR_CC_COMMON_OK)   return res;
        self->root.size--;
    }

    adapter_free(self);
    return ERR_CC_COMMON_OK;
}

/*
A: roota -> a1 -> a2 -> a3 -> a4 -> roota
B: rootb -> b1 -> b2 -> b3 -> b4 -> rootb
A + B:
A: roota-> a1 -> a2 -> a3 -> a4 -> b1 -> b2 -> b3 -> b4 -> roota
roota.size = roota.size + rootb.size
B: rootb-> rootb
*/

int cc_list_concat(cc_list_t *left, cc_list_t *right)
{
    if(left == NULL || right == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if(right->root.size == 0) {
        return ERR_CC_COMMON_OK;
    }

    if(left->root.size == 0) {
        left->root.next = right->root.next;
        left->root.prev = right->root.prev;
        right->root.next->prev = &left->root;
        right->root.prev->next = &left->root;
    }
    else {
        left->root.prev->next = right->root.next;
        right->root.next->prev = left->root.prev;

        left->root.prev = right->root.prev;
        right->root.prev->next = &left->root;
    }
    left->root.size += right->root.size;

    cc_list_init(right, right->remove_fn);

    return ERR_CC_COMMON_OK;
}

int cc_list_concat_by_cmp(cc_list_t *left, cc_list_t *right, cc_cmp_fn_t cmp) {
    if (left == NULL || right == NULL || cmp == NULL)
        return ERR_CC_COMMON_INVALID_ARG;

    // ➤ 空链表快速返回
    if (cc_list_size(right) == 0) {
        return ERR_CC_COMMON_OK; // 右链表为空，直接保留左链表
    }
    if (cc_list_size(left) == 0) {
        // 左链表为空时，全量复制右链表节点到左
        left->root.next = right->root.next;
        left->root.prev = right->root.prev;
        right->root.next->prev = &left->root;
        right->root.prev->next = &left->root;
        left->root.size = right->root.size;

        cc_list_init(right, right->remove_fn);
        return ERR_CC_COMMON_OK;
    }

    // ➤ 创建哨兵节点（临时头，统一操作逻辑）
    cc_list_node_t sentinel;
    sentinel.next = &sentinel;
    sentinel.prev = &sentinel;

    // ➤ 双指针模式接管节点，无需新建节点
    cc_list_node_t *l_ptr = left->root.next;
    cc_list_node_t *r_ptr = right->root.next;
    cc_list_node_t *tail = &sentinel;

    // ▶ 合并两链表主循环
    while (l_ptr != &left->root && r_ptr != &right->root) {
        int res = cmp(l_ptr->data, r_ptr->data);
        if (res <= 0) { // 左链表值较小或相等 → 插入左节点
            // 从原链表断链
            l_ptr->prev->next = l_ptr->next;
            l_ptr->next->prev = l_ptr->prev;

            // 添加到合并链尾部
            l_ptr->prev = tail;
            l_ptr->next = tail->next;
            tail->next = l_ptr;
            l_ptr->next->prev = l_ptr;
            tail = l_ptr; // 尾指针更新

            l_ptr = left->root.next; // 重新指向左链表头节点下一个
        } else { // 右链表值较小 → 插入右节点
            // 从原链表断链
            r_ptr->prev->next = r_ptr->next;
            r_ptr->next->prev = r_ptr->prev;

            // 添加到合并链尾部
            r_ptr->prev = tail;
            r_ptr->next = tail->next;
            tail->next = r_ptr;
            r_ptr->next->prev = r_ptr;
            tail = r_ptr; // 尾指针更新

            r_ptr = right->root.next; // 重新指向右链表头节点下一个
        }
    }

    // ▶ 处理剩余节点（左或右链表尚未遍历完）
    // 左链表剩余
    while (l_ptr != &left->root) {
        l_ptr->prev->next = l_ptr->next;
        l_ptr->next->prev = l_ptr->prev;

        l_ptr->prev = tail;
        l_ptr->next = tail->next;
        tail->next = l_ptr;
        l_ptr->next->prev = l_ptr;
        tail = l_ptr;

        l_ptr = left->root.next;
    }
    // 右链表剩余
    while (r_ptr != &right->root) {
        r_ptr->prev->next = r_ptr->next;
        r_ptr->next->prev = r_ptr->prev;

        r_ptr->prev = tail;
        r_ptr->next = tail->next;
        tail->next = r_ptr;
        r_ptr->next->prev = r_ptr;
        tail = r_ptr;

        r_ptr = right->root.next;
    }

    // ▶ 更新左链表并恢复循环结构
    left->root.prev = tail;
    tail->next = &left->root;
    sentinel.next->prev = &left->root;
    left->root.next = sentinel.next;

    left->root.size += right->root.size;

    cc_list_init(right, right->remove_fn);
    return ERR_CC_COMMON_OK;
}

int cc_list_concat_ab_by_cmp_to_c(cc_list_t *merge, cc_list_t *left, cc_list_t *right, cc_cmp_fn_t cmp)
{
    if (left == NULL || right == NULL || cmp == NULL)
        return ERR_CC_COMMON_INVALID_ARG;

    // ➤ 创建哨兵节点（临时头，统一操作逻辑）
    cc_list_node_t sentinel;
    sentinel.next = &sentinel;
    sentinel.prev = &sentinel;

    // ➤ 双指针模式接管节点，无需新建节点
    cc_list_node_t *l_ptr = left->root.next;
    cc_list_node_t *r_ptr = right->root.next;
    cc_list_node_t *tail = &sentinel;

    // ▶ 合并两链表主循环
    while (l_ptr != &left->root && r_ptr != &right->root) {
        int res = cmp(l_ptr->data, r_ptr->data);
        if (res <= 0) { // 左链表值较小或相等 → 插入左节点
            // 从原链表断链
            l_ptr->prev->next = l_ptr->next;
            l_ptr->next->prev = l_ptr->prev;

            // 添加到合并链尾部
            l_ptr->prev = tail;
            l_ptr->next = tail->next;
            tail->next = l_ptr;
            l_ptr->next->prev = l_ptr;
            tail = l_ptr; // 尾指针更新

            l_ptr = left->root.next; // 重新指向左链表头节点下一个
        } else { // 右链表值较小 → 插入右节点
            // 从原链表断链
            r_ptr->prev->next = r_ptr->next;
            r_ptr->next->prev = r_ptr->prev;

            // 添加到合并链尾部
            r_ptr->prev = tail;
            r_ptr->next = tail->next;
            tail->next = r_ptr;
            r_ptr->next->prev = r_ptr;
            tail = r_ptr; // 尾指针更新

            r_ptr = right->root.next; // 重新指向右链表头节点下一个
        }
    }

    // ▶ 处理剩余节点（左或右链表尚未遍历完）
    // 左链表剩余
    while (l_ptr != &left->root) {
        l_ptr->prev->next = l_ptr->next;
        l_ptr->next->prev = l_ptr->prev;

        l_ptr->prev = tail;
        l_ptr->next = tail->next;
        tail->next = l_ptr;
        l_ptr->next->prev = l_ptr;
        tail = l_ptr;

        l_ptr = left->root.next;
    }
    // 右链表剩余
    while (r_ptr != &right->root) {
        r_ptr->prev->next = r_ptr->next;
        r_ptr->next->prev = r_ptr->prev;

        r_ptr->prev = tail;
        r_ptr->next = tail->next;
        tail->next = r_ptr;
        r_ptr->next->prev = r_ptr;
        tail = r_ptr;

        r_ptr = right->root.next;
    }

    // ▶ 更新左链表并恢复循环结构
    merge->root.prev = tail;
    tail->next = &merge->root;
    sentinel.next->prev = &merge->root;
    merge->root.next = sentinel.next;

    merge->root.size += right->root.size;
    merge->root.size += left->root.size;

    cc_list_init(right, right->remove_fn);
    cc_list_init(left, left->remove_fn);
    return ERR_CC_COMMON_OK;
}


int cc_list_split(cc_list_t **new_list, cc_list_t *old_list, cc_check_fn_t check_fn)
{
    if(new_list == NULL || *new_list != NULL || old_list == NULL || check_fn == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }
    int res = ERR_CC_COMMON_OK;
    cc_list_t *temp_list = NULL;
    res = cc_list_new(&temp_list, old_list->remove_fn);
    if(res != ERR_CC_COMMON_OK) return res;

    cc_list_node_t *old_list_node = old_list->root.next;
    while(old_list_node != &old_list->root) {
        cc_list_node_t *next_node = old_list_node->next;
        if(check_fn(old_list_node->data) == ERR_CC_COMMON_OK) {
            old_list_node->prev->next = old_list_node->next;
            old_list_node->next->prev = old_list_node->prev;
            old_list->root.size--;

            old_list_node->prev = temp_list->root.prev;
            old_list_node->next = &temp_list->root;
            temp_list->root.prev->next = old_list_node;
            temp_list->root.prev = old_list_node;
            temp_list->root.size++;
        }
        old_list_node = next_node;
    }

    *new_list = temp_list;
    return ERR_CC_COMMON_OK;
}

/*
    begin:  root1->a->b->c->d->e->root1
    run:        slow a b c
                fast b d root1 over
    end:    root1->a->b->root1
    end:    root2->c->d->e->root2
    end:    slow: c
    end:    fast: root1
need_change:
    root1->prev, root1->prev->next,
    slow->prev->next, slow->prev,
    root2->prev, root2->next,
change:


    root2->prev = root1->prev; 1 // e<-root2
    root2->next = slow;        2 // e<-root2->c
    root1->prev->next = root2; 3 // e-root2->c
    root1->prev = slow->prev;  4 // b<-root1
    slow->prev->next = root1;  5 // b-root1
    slow->prev = root2;        6 // e-root2-c


*/

int cc_list_split_middle(cc_list_t **new_right_list, cc_list_t *old_left_list)
{
    if(new_right_list == NULL || *new_right_list != NULL || old_left_list == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if(cc_list_size(old_left_list) < 2) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    int res = ERR_CC_COMMON_OK;
    cc_list_t *temp_list = NULL;
    res = cc_list_new(&temp_list, old_left_list->remove_fn);
    if(res != ERR_CC_COMMON_OK) return res;

    cc_list_node_t *slow,*fast;
    slow = fast = old_left_list->root.next;
    while(fast != &old_left_list->root && fast->next != &old_left_list->root) {
        slow = slow->next;
        fast = fast->next->next;
    }

    temp_list->root.prev = old_left_list->root.prev;
    temp_list->root.next = slow;

    old_left_list->root.prev->next = &temp_list->root;
    old_left_list->root.prev = slow->prev;

    slow->prev->next = &old_left_list->root;
    slow->prev = &temp_list->root;

    temp_list->root.size = old_left_list->root.size - (old_left_list->root.size >> 1);
    old_left_list->root.size = old_left_list->root.size >> 1;

    *new_right_list = temp_list;
    return ERR_CC_COMMON_OK;
}

int cc_list_split_block(cc_list_t **new_block_list, cc_list_t *old_left_list, cc_size_t block_size, cc_list_node_t **current)
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *temp_block_list;
    res = cc_list_new(&temp_block_list, old_left_list->remove_fn);
    if(res != ERR_CC_COMMON_OK) return res;
    for(cc_size_t i = 0; (i < block_size) && (*current != &old_left_list->root); i++) {
        cc_list_node_t *next_node = (*current)->next;

        (*current)->prev->next = (*current)->next;
        (*current)->next->prev = (*current)->prev;
        old_left_list->root.size--;

        (*current)->prev = temp_block_list->root.prev;
        (*current)->next = &temp_block_list->root;
        temp_block_list->root.prev->next = (*current);
        temp_block_list->root.prev = (*current);
        temp_block_list->root.size++;

        (*current) = next_node;
    }
    *new_block_list = temp_block_list;
    return res;
}

int cc_list_copy(cc_list_t **new_list, cc_list_t *old_list, cc_copy_data_fn_t copy_data)
{
    int res = ERR_CC_COMMON_OK;
    if(new_list == NULL || copy_data == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }
    *new_list = NULL;

    cc_list_t *temp_list = NULL;
    res = cc_list_new(&temp_list, old_list->remove_fn);
    if(res != ERR_CC_COMMON_OK) {
        goto fail1;
    }


    cc_list_node_t *temp_list_node = old_list->root.next;
    while(temp_list_node != &old_list->root) {
        void *data_ptr = NULL;
        res = copy_data(temp_list_node->data, &data_ptr);
        if(res != ERR_CC_COMMON_OK) {
            goto fail2;
        }
        if(data_ptr == NULL) {
            res = ERR_CC_COMMON_MEM_ERR;
            goto fail2;
        }

        res = cc_list_insert_tail(temp_list, data_ptr);
        if(res != ERR_CC_COMMON_OK) {
            goto fail2;
        }
        temp_list_node = temp_list_node->next;
    }
    *new_list = temp_list;
    return ERR_CC_COMMON_OK;

fail2:
    cc_list_destroy(temp_list);
fail1:
    return res;
}

int cc_list_insert_head(cc_list_t *self, void *data)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    res =  cc_list_node_insert_after(&self->root, data);

    if(res == ERR_CC_COMMON_OK) {
        self->root.size++;
    }
    return res;
}

int cc_list_insert_tail(cc_list_t *self, void *data)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    res = cc_list_node_insert_before(&self->root, data);

    if(res == ERR_CC_COMMON_OK) {
        self->root.size++;
    }
    return res;
}

int cc_list_insert_node_head(cc_list_t *self, cc_list_node_t *insert_node)
{
    int res = ERR_CC_COMMON_OK;
    if((self == NULL) || (insert_node == NULL)) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    res =  cc_list_node_insert_node(insert_node, &self->root, self->root.next);

    if(res == ERR_CC_COMMON_OK) {
        self->root.size++;
    }
    return res;
}

int cc_list_insert_node_tail(cc_list_t *self, cc_list_node_t *insert_node)
{
    int res = ERR_CC_COMMON_OK;
    if((self == NULL) || (insert_node == NULL)) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    res =  cc_list_node_insert_node(insert_node, self->root.prev, &self->root);

    if(res == ERR_CC_COMMON_OK) {
        self->root.size++;
    }
    return res;
}


//未处理 链表总node个数的增减 需要注意 与其他的插入混用时会导致链表size不正确
int cc_list_insert_node(cc_list_node_t *insert_node, cc_list_node_t *prev, cc_list_node_t *next)
{
    int res = ERR_CC_COMMON_OK;
    if(insert_node == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    next->prev = insert_node;
    insert_node->next = next;
    insert_node->prev = prev;
    prev->next = insert_node;
    return res;
}

int cc_list_insert_sort(cc_list_t *self, void *data, cc_cmp_fn_t cmp)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL || cmp == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if(self->root.size == 0) {
        res = cc_list_insert_tail(self, data);
        return res;
    }

    cc_list_node_t *temp = self->root.next;
    int is_insert = 0;
    while(temp != &self->root) {
        if(cmp(temp->data, data) > 0) {
            res = cc_list_node_insert_after(temp, data);
            is_insert = 1;
            break;
        }
        temp = temp->next;
    }

    if(!is_insert) {
        res = cc_list_insert_tail(self,data);
    }

    return res;
}

int cc_list_remove_head(cc_list_t *self, void **data)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if(cc_list_is_empty(self)) {
        return ERR_CC_LIST_EMPTY;
    }

    res = cc_list_node_remove_after(&self->root, data);

    if(res == ERR_CC_COMMON_OK) {
        self->root.size--;
    }
    return res;
}

int cc_list_remove_tail(cc_list_t *self, void **data)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if(cc_list_is_empty(self)) {
        return ERR_CC_LIST_EMPTY;
    }

    res = cc_list_node_remove_before(&self->root, data);

    if(res == ERR_CC_COMMON_OK) {
        self->root.size--;
    }

    return res;
}

int cc_list_remove_node(cc_list_t *self, cc_list_node_t *node, void **data)
{
    if (self == NULL || node == NULL) return ERR_CC_LIST_INVALID_ARG;

    // Check if node is within this list
    cc_list_node_t *curr;
    int found = 0;
    cc_list_for_each(curr, &(self->root)) {
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
    self->root.size--;

    // Free the node itself (but not the data)
    free(node);

    return ERR_CC_LIST_OK;
}

int cc_list_get_head(cc_list_t *self, void **data)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL || data == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if(cc_list_is_empty(self)) {
        return ERR_CC_LIST_EMPTY;
    }

    *data = self->root.next->data;

    return res;
}

int cc_list_get_tail(cc_list_t *self, void **data)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL || data == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if(cc_list_is_empty(self)) {
        return ERR_CC_LIST_EMPTY;
    }

    *data = self->root.prev->data;

    return res;
}

int cc_list_root_swap(cc_list_t *self, cc_list_t *others)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL || others == NULL) {
        return ERR_CC_COMMON_INVALID_ARG;
    }

    if(self->root.size == 0) {
        self->root.next = others->root.next;
        self->root.prev = others->root.prev;
        self->root.size = others->root.size;

        others->root.prev->next = &self->root;
        others->root.next->prev = &self->root;

        cc_list_init(others, others->remove_fn);
        return ERR_CC_COMMON_OK;
    }

    if(others->root.size == 0) {
        others->root.next = self->root.next;
        others->root.prev = self->root.prev;
        others->root.size = self->root.size;

        self->root.prev->next = &others->root;
        self->root.next->prev = &others->root;

        cc_list_init(self, others->remove_fn);
        return ERR_CC_COMMON_OK;
    }

    cc_list_node_t temp_node;
    temp_node.next = self->root.next;
    temp_node.prev = self->root.prev;
    temp_node.size = self->root.size;

    temp_node.prev->next = &others->root;
    temp_node.next->prev = &others->root;

    self->root.next = others->root.next;
    self->root.prev = others->root.prev;
    self->root.size = others->root.size;

    others->root.prev->next = &self->root;
    others->root.next->prev = &self->root;

    others->root.prev = temp_node.prev;
    others->root.next = temp_node.next;
    others->root.size = temp_node.size;
    return res;
}

int cc_list_is_empty(cc_list_t *self)
{
    return self->root.next == &self->root;
}

inline cc_size_t cc_list_size(cc_list_t *self)
{
    return self->root.size;
}

int cc_list_print_for(cc_list_t *self, int direction, cc_debug_print_fn_t cc_debug_print)
{
    cc_size_t len = self->root.size;
    cc_list_node_t *temp = NULL;
    if(direction == 1) {
        temp = self->root.next;
        for(cc_size_t i = 0; i < len; i++)
        {
            if(cc_debug_print != NULL)
                cc_debug_print(temp->data);
            temp = temp->next;
        }
    }
    else {
        temp = self->root.prev;
        for(cc_size_t i = 0; i < len; i++)
        {
            if(cc_debug_print != NULL)
                cc_debug_print(temp->data);
            temp = temp->prev;
        }
    }
    return ERR_CC_COMMON_OK;
}


int cc_list_print(cc_list_t *self, int direction, cc_debug_print_fn_t cc_debug_print)
{
    return cc_list_print_for(self, direction, cc_debug_print);
}


cc_iter_i_t cc_list_iter_interface = {
    .next = (cc_iter_next_fn_t)cc_list_iter_next,
};

static inline void cc_list_iter_step(cc_list_iterator_t *self)
{
    if(self->direction) {
        self->cursor = self->cursor->next;
    }
    else {
        self->cursor = self->cursor->prev;
    }
}

int cc_list_iter_init(cc_list_iterator_t *self, cc_list_t *list, int direction)
{
    if(self == NULL || list == NULL) return ERR_CC_COMMON_INVALID_ARG;
    if((direction != 0) && (direction != 1)) return ERR_CC_COMMON_INVALID_ARG;

    self->interface = &cc_list_iter_interface;
    self->list = list;
    self->index = 0;
    self->direction = direction;
    self->cursor = &self->list->root;

    cc_list_iter_step(self);
    return ERR_CC_COMMON_OK;
}

int cc_list_iter_new(cc_list_iterator_t **self, cc_list_t *list, int direction)
{
    int res = ERR_CC_COMMON_OK;
    cc_list_iterator_t *temp = malloc(sizeof(*temp));
    if(temp == NULL) return ERR_CC_COMMON_MEM_ERR;

    res = cc_list_iter_init(temp, list, direction);
    if (res != ERR_CC_COMMON_OK) {
        free(temp);
        return res;
    }

    *self = temp;
    return ERR_CC_COMMON_OK;
}

int cc_list_iter_delete(cc_list_iterator_t *self)
{
    if(self == NULL) return ERR_CC_COMMON_INVALID_ARG;
    free(self);
    return ERR_CC_COMMON_OK;
}

int cc_list_iter_next(cc_list_iterator_t *self, void **item, cc_size_t *index)
{
    if(self == NULL) return ERR_CC_COMMON_INVALID_ARG;
    if(try_reset_double_p(item) != ERR_CC_COMMON_OK) return ERR_CC_COMMON_INVALID_ARG;

    if(self->cursor == &self->list->root) {
        return ERR_CC_ITER_END;
    }

    *item = &self->cursor->data;

    if(index != NULL) {
        *index = self->index;
    }

    self->index++;
    cc_list_iter_step(self);

    return ERR_CC_COMMON_OK;
}
#include "ds/list/cc_list.h"
#include "cc_stdint.h"
#include "core/cc_common.h"
#include "core/cc_mem.h"
#include <string.h>
#include "core/cc_dbg.h"

int cc_list_node_insert_before(cc_list_node_t *self, void *data)
{
    cc_list_node_t *temp;

    temp = (cc_list_node_t *)cc_malloc(sizeof(cc_list_node_t));
    if (temp == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    temp->data = data;
    temp->prev = self->prev;
    temp->next = self;
    self->prev->next = temp;
    self->prev = temp;

    return ERR_CC_LIST_OK;
}

int cc_list_node_insert_after(cc_list_node_t *self, void *data)
{
    cc_list_node_t *temp;

    temp = (cc_list_node_t *)cc_malloc(sizeof(cc_list_node_t));
    if (temp == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    temp->data = data;
    temp->next = self->next;
    temp->prev = self;
    self->next->prev = temp;
    self->next = temp;

    return ERR_CC_LIST_OK;
}

int cc_list_node_remove_before(cc_list_node_t *self, void **result)
{
    cc_list_node_t *temp;

    if (self->prev == self) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    temp = self->prev;
    *result = temp->data;

    temp->prev->next = self;
    self->prev = temp->prev;


    cc_free(temp);
    return ERR_CC_LIST_OK;
}

int cc_list_node_remove_after(cc_list_node_t *self, void **result)
{
    cc_list_node_t *temp;

    if(self->next == self) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    temp = self->next;
    *result = temp->data;


    temp->next->prev = self;
    self->next = temp->next;

    cc_free(temp);
    return ERR_CC_LIST_OK;
}

/*
    list: ...->others_before->current->others_after->....
    cc_list_node_t *temp = current
    cc_list_node_delete_and_next(&temp);
    result: temp = others_after cc_free(current);
    list: ...->others_before->others_after->....
*/
int cc_list_node_delete_and_next(cc_list_node_t **current, cc_delete_fn_t remove_fn)
{




    cc_list_node_t *temp, *next;
    void *data;
    if(*current == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
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
    cc_free(temp);

    *current = next;
    return ERR_CC_LIST_OK;
}


int cc_list_init(cc_list_t *self, cc_delete_fn_t remove_fn)
{
    if(self == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    self->root.prev = &self->root;
    self->root.next = &self->root;
    self->root.size = 0;
    self->remove_fn = remove_fn;
    return ERR_CC_LIST_OK;
}

int cc_list_new(cc_list_t **self, cc_delete_fn_t remove_fn)
{
    cc_list_t *temp;
    temp = (cc_list_t *)cc_malloc(sizeof(cc_list_t));
    if (temp == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    cc_list_init(temp, remove_fn);

    *self = temp;
    return ERR_CC_LIST_OK;
}

int cc_list_destroy(cc_list_t *self)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }
    cc_list_node_t *temp;

    temp = self->root.next;

    while(temp != &self->root) {
        res = cc_list_node_delete_and_next(&temp, self->remove_fn);
        if(res != ERR_CC_LIST_OK)   return res;
        self->root.size--;
    }

    cc_free(self);
    return ERR_CC_LIST_OK;
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
        return ERR_CC_LIST_INVALID_ARG;
    }

    if(right->root.size == 0) {
        return ERR_CC_LIST_OK;
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

    return ERR_CC_LIST_OK;
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
    return ERR_CC_LIST_OK;

fail2:
    cc_list_destroy(temp_list);
fail1:
    return res;
}

int cc_list_insert_head(cc_list_t *self, void *data)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    res =  cc_list_node_insert_after(&self->root, data);

    if(res == ERR_CC_LIST_OK) {
        self->root.size++;
    }
    return res;
}

int cc_list_insert_tail(cc_list_t *self, void *data)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    res = cc_list_node_insert_before(&self->root, data);

    if(res == ERR_CC_LIST_OK) {
        self->root.size++;
    }
    return res;
}

int cc_list_remove_head(cc_list_t *self, void **data)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    if(cc_list_is_empty(self)) {
        return ERR_CC_LIST_EMPTY;
    }

    res = cc_list_node_remove_after(&self->root, data);

    if(res == ERR_CC_LIST_OK) {
        self->root.size--;
    }
    return res;
}

int cc_list_remove_tail(cc_list_t *self, void **data)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    if(cc_list_is_empty(self)) {
        return ERR_CC_LIST_EMPTY;
    }

    res = cc_list_node_remove_before(&self->root, data);

    if(res == ERR_CC_LIST_OK) {
        self->root.size--;
    }

    return res;
}

int cc_list_get_head(cc_list_t *self, void **data)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL || data == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    if(cc_list_is_empty(self)) {
        return ERR_CC_LIST_EMPTY;
    }

    *data = self->root.next->data;

    return res;
}

int cc_list_get_tail(cc_list_t *self, void **data)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL || data == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    if(cc_list_is_empty(self)) {
        return ERR_CC_LIST_EMPTY;
    }

    *data = self->root.prev->data;

    return res;
}

int cc_list_is_empty(cc_list_t *self)
{
    return self->root.next == &self->root;
}

cc_size_t cc_list_size(cc_list_t *self)
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
    return ERR_CC_LIST_OK;
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
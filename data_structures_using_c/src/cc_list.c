#include "cc_list.h"
#include <stdlib.h>

int cc_list_node_insert_before(cc_list_node_t *self, void *data)
{
    cc_list_node_t *temp;

    temp = (cc_list_node_t *)malloc(sizeof(cc_list_node_t));
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

    temp = (cc_list_node_t *)malloc(sizeof(cc_list_node_t));
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


    free(temp);
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

    free(temp);
    return ERR_CC_LIST_OK;
}

int cc_list_node_delete_and_next(struct cc_list_node **current, cc_delete_fn_t remove_fn)
{

/*
    list: ...->others_before->current->others_after->....
    cc_list_node_t *temp = current
    cc_list_node_delete_and_next(&temp);
    result: temp = others_after free(current);
    list: ...->others_before->others_after->....
*/


    cc_list_node_t *temp, *next;

    if(*current == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    temp = *current;
    next = temp->next;

    if(remove_fn != NULL) {
        remove_fn(temp->data);
    }

    temp->prev->next = next;
    next->prev = temp->prev;
    free(temp);

    *current = next;
    return ERR_CC_LIST_OK;
}


int cc_list_init(cc_list_t *self)
{
    if(self == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    self->root.prev = &self->root;
    self->root.next = &self->root;
    self->root.size = 0;

    return ERR_CC_LIST_OK;
}
int cc_list_new(cc_list_t **self)
{
    cc_list_t *temp;
    temp = (cc_list_t *)malloc(sizeof(cc_list_t));
    if (temp == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    cc_list_init(temp);

    *self = temp;
    return ERR_CC_LIST_OK;
}
int cc_list_destroy(cc_list_t *self, cc_delete_fn_t remove_fn)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }
    cc_list_node_t *temp;

    temp = self->root.next;

    while(temp != &self->root) {
        res = cc_list_node_delete_and_next(&temp, remove_fn);
        if(res != ERR_CC_LIST_OK)
        return res;
    }

    free(self);
    return ERR_CC_LIST_OK;
}

int cc_list_concat(struct cc_list *left, struct cc_list *right)
{
    if(left == NULL) {
        return ERR_CC_LIST_INVALID_ARG;
    }

    if(right == NULL || right->root.size == 0) {
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

    cc_list_init(right);

    return ERR_CC_LIST_OK;
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
size_t cc_list_size(cc_list_t *self)
{
    return self->root.size;
}

int cc_list_print_for(cc_list_t *self, int direction, cc_debug_print_fn_t cc_debug_print)
{
    size_t len = self->root.size;
    cc_list_node_t *temp = NULL;
    if(direction == 1) {
        temp = self->root.next;
        for(size_t i = 0; i < len; i++)
        {
            cc_debug_print(temp->data);
            temp = temp->next;
        }
    }
    else {
        temp = self->root.prev;
        for(size_t i = 0; i < len; i++)
        {
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

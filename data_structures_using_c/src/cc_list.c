#include "cc_list.h"
#include "cc_common.h"
#include "cc_mem.h"

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

int cc_list_node_delete_and_next(cc_list_node_t **current, cc_delete_fn_t remove_fn)
{

/*
    list: ...->others_before->current->others_after->....
    cc_list_node_t *temp = current
    cc_list_node_delete_and_next(&temp);
    result: temp = others_after cc_free(current);
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
        if(res != ERR_CC_LIST_OK)
        return res;
    }

    cc_free(self);
    return ERR_CC_LIST_OK;
}

int cc_list_concat(cc_list_t *left, cc_list_t *right)
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

    cc_list_init(right, right->remove_fn);

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
            cc_debug_print(temp->data);
            temp = temp->next;
        }
    }
    else {
        temp = self->root.prev;
        for(cc_size_t i = 0; i < len; i++)
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
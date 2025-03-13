#include "cc_common.h"
#include "cc_mem.h"

#include "cc_list.h"
#include "cc_list_queue.h"

cc_queue_i_t cc_list_queue_interface = {
    .enqueue = (cc_queue_enqueue_fn_t) cc_list_queue_enqueue,
    .dequeue = (cc_queue_dequeue_fn_t) cc_list_queue_dequeue,
    .peek = (cc_queue_peek_fn_t) cc_list_queue_peek,
    .is_empty = (cc_queue_is_empty_fn_t) cc_list_queue_is_empty,
    .is_full = (cc_queue_is_full_fn_t) cc_list_queue_is_full,
    .size = (cc_queue_size_fn_t) cc_list_queue_size,
};

int cc_list_queue_new(cc_list_queue_t **self, cc_delete_fn_t remove_fn)
{
    int res = ERR_CC_COMMON_OK;
    cc_list_queue_t *temp = malloc(sizeof(*temp));
    if(temp == NULL) {
        res = ERR_CC_COMMON_MEM_ERR;
        goto fail1;
    }

    res = cc_list_new(&temp->list, remove_fn);
    if(res != ERR_CC_COMMON_OK) {
        goto fail2;
    }

    temp->interface = &cc_list_queue_interface;
    *self = temp;
    return ERR_CC_LIST_OK;

fail2:
    adapter_free(temp);
fail1:
    return res;
}

int cc_list_queue_delete(cc_list_queue_t *self)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) return ERR_CC_LIST_INVALID_ARG;

    res = cc_list_destroy(self->list);
    if(res != ERR_CC_COMMON_OK) {
        return res;
    }

    adapter_free(self);


    return ERR_CC_COMMON_OK;
}

int cc_list_queue_enqueue(cc_list_queue_t *self, void *data)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) return ERR_CC_LIST_INVALID_ARG;

    res = cc_list_insert_tail(self->list, data);
    return res;
}

int cc_list_queue_dequeue(cc_list_queue_t *self, void **result)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) return ERR_CC_LIST_INVALID_ARG;

    res = cc_list_remove_head(self->list, result);
    return res;
}

int cc_list_queue_peek(cc_list_queue_t *self, void **result)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) return ERR_CC_LIST_INVALID_ARG;

    res = cc_list_get_head(self->list, result);
    return res;
}

int cc_list_queue_is_empty(cc_list_queue_t *self)
{
    return cc_list_is_empty(self->list);
}

//because of created by list.
int cc_list_queue_is_full(cc_list_queue_t *self)
{
    NOTUSED(self);
    return ERR_CC_COMMON_NOT_DEFINE;
}

cc_size_t cc_list_queue_size(cc_list_queue_t *self)
{
    return cc_list_size(self->list);
}


#include "cc_common.h"
#include "cc_list.h"
#include "cc_stack.h"
#include "cc_list_stack.h"
#include "cc_dbg.h"
#include "cc_mem.h"

struct cc_stack_i cc_list_stack_interface = {
    .push = (cc_stack_push_fn_t)cc_list_stack_push,
    .pop = (cc_stack_pop_fn_t)cc_list_stack_pop,
    .peek = (cc_stack_peek_fn_t)cc_list_stack_peek,
};

int cc_list_stack_new(cc_list_stack_t **self, cc_delete_fn_t remove_fn)
{
    int res = ERR_CC_LIST_OK;
    cc_list_stack_t *temp = NULL;

    temp = cc_malloc(sizeof(*temp));
    if(temp == NULL) {
        res = ERR_CC_LIST_MEM_ERR;
        goto fail1;
    }

    res = cc_list_new(&temp->list, remove_fn);
    if(res != ERR_CC_LIST_OK) {
        goto fail2;
    }

    temp->interface = &cc_list_stack_interface;

    *self = temp;
    return ERR_CC_LIST_OK;

fail2:
    cc_free(temp);
fail1:
    return res;
}

int cc_list_stack_delete(cc_list_stack_t *self)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL) return res;

    if(self->list != NULL) {
        res = cc_list_destroy(self->list);
        if(res != ERR_CC_LIST_OK) return res;
    }

    self->interface = NULL;
    cc_free(self);
    return res;
}

int cc_list_stack_push(cc_list_stack_t *self, void *data)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL || self->list == NULL) return ERR_CC_LIST_INVALID_ARG;

    res = cc_list_insert_tail(self->list, data);

    return res;
}

int cc_list_stack_pop(cc_list_stack_t *self, void **result)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL || self->list == NULL) return ERR_CC_LIST_INVALID_ARG;

    res = cc_list_remove_tail(self->list, result);

    if(res == ERR_CC_LIST_EMPTY) {
        res = ERR_CC_STACK_EMPTY;
    }

    return res;
}

int cc_list_stack_peek(cc_list_stack_t *self, void **result)
{
    int res = ERR_CC_LIST_OK;
    if(self == NULL || self->list == NULL) return ERR_CC_LIST_INVALID_ARG;

    res = cc_list_get_tail(self->list, result);
    if(res == ERR_CC_LIST_EMPTY) {
        res = ERR_CC_STACK_EMPTY;
    }

    return res;
}


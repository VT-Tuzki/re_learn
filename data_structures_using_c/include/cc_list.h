#ifndef __CC_LIST_H__
#define __CC_LIST_H__

#include "cc_common.h"
#include "cc_array.h"
#include "cc_stdint.h"

struct cc_list_node {
    struct cc_list_node *next;
    struct cc_list_node *prev;
    union {
        void *data; // when self not head
        cc_size_t size; // when self is head (size of list)
    };
};
typedef struct cc_list_node cc_list_node_t;

struct cc_list {
    cc_list_node_t root;
};
typedef struct cc_list cc_list_t;

typedef enum {
    ERR_CC_LIST_OK = ERR_CC_COMMON_OK,
    ERR_CC_LIST_INVALID_ARG = ERR_CC_COMMON_INVALID_ARG,
    ERR_CC_LIST_MEM_ERR = ERR_CC_COMMON_MEM_ERR,
    ERR_CC_LIST_EMPTY,
    ERR_CC_LIST_STACK_BEGIN = 16,

} cc_list_err;


int cc_list_node_insert_before(cc_list_node_t *self, void *data);
int cc_list_node_insert_after(cc_list_node_t *self, void *data);
int cc_list_node_remove_before(cc_list_node_t *self, void **result);
int cc_list_node_remove_after(cc_list_node_t *self, void **result);

int cc_list_node_delete_and_next(cc_list_node_t **current, cc_delete_fn_t remove_fn);



int cc_list_init(cc_list_t *self);
int cc_list_new(cc_list_t **self);
int cc_list_destroy(cc_list_t *self, cc_delete_fn_t remove_fn);

int cc_list_concat(cc_list_t *left, cc_list_t *right);

int cc_list_insert_head(cc_list_t *self, void *data);
int cc_list_insert_tail(cc_list_t *self, void *data);
int cc_list_remove_head(cc_list_t *self, void **data);
int cc_list_remove_tail(cc_list_t *self, void **data);
int cc_list_get_head(cc_list_t *self, void **data);
int cc_list_get_tail(cc_list_t *self, void **data);

int cc_list_is_empty(cc_list_t *self);
cc_size_t cc_list_size(cc_list_t *self);

int cc_list_to_cc_array(cc_list_t *self, cc_array_t **result);

int cc_list_print(cc_list_t *self, int direction, cc_debug_print_fn_t cc_debug_print);



#endif // __CC_LIST_H__
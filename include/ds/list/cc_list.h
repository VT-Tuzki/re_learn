#ifndef __CC_LIST_H__
#define __CC_LIST_H__

#include "../../core/cc_common.h"
#include "../../core/cc_stdint.h"

#include "../array/cc_array.h"
#include "../iter/cc_iter.h"


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
    cc_delete_fn_t remove_fn;
};
typedef struct cc_list cc_list_t;

typedef enum {
    ERR_CC_LIST_OK = ERR_CC_COMMON_OK,
    ERR_CC_LIST_INVALID_ARG = ERR_CC_COMMON_INVALID_ARG,
    ERR_CC_LIST_MEM_ERR = ERR_CC_COMMON_MEM_ERR,
    ERR_CC_LIST_EMPTY,
    ERR_CC_LIST_CYCLE,

} cc_list_err;


int cc_list_node_insert_before(cc_list_node_t *self, void *data);
int cc_list_node_insert_after(cc_list_node_t *self, void *data);
int cc_list_node_remove_before(cc_list_node_t *self, void **result);
int cc_list_node_remove_after(cc_list_node_t *self, void **result);

int cc_list_node_delete_and_next(cc_list_node_t **current, cc_delete_fn_t remove_fn);



int cc_list_init(cc_list_t *self, cc_delete_fn_t remove_fn);
int cc_list_new(cc_list_t **self, cc_delete_fn_t remove_fn);
int cc_list_destroy(cc_list_t *self);

int cc_list_concat(cc_list_t *left, cc_list_t *right);
int cc_list_concat_by_cmp(cc_list_t *left, cc_list_t *right, cc_cmp_fn_t cmp);
int cc_list_concat_ab_by_cmp_to_c(cc_list_t *merge, cc_list_t *left, cc_list_t *right, cc_cmp_fn_t cmp);


int cc_list_copy(cc_list_t **new_list, cc_list_t *old_list, cc_copy_data_fn_t copy_fn);
int cc_list_split(cc_list_t **new_list, cc_list_t *old_list, cc_check_fn_t check_fn);
int cc_list_split_middle(cc_list_t **new_right_list, cc_list_t *old_left_list);
int cc_list_split_block(cc_list_t **new_block_list, cc_list_t *old_left_list, cc_size_t block_size, cc_list_node_t **current);
int cc_list_split_block_no_malloc(cc_list_t *new_block_list, cc_list_t *old_left_list, cc_size_t block_size, cc_list_node_t **current);

int cc_list_insert_head(cc_list_t *self, void *data);
int cc_list_insert_tail(cc_list_t *self, void *data);
int cc_list_insert_sort(cc_list_t *self, void *data, cc_cmp_fn_t cmp);


int cc_list_remove_head(cc_list_t *self, void **data);
int cc_list_remove_tail(cc_list_t *self, void **data);
int cc_list_get_head(cc_list_t *self, void **data);
int cc_list_get_tail(cc_list_t *self, void **data);
int cc_list_root_swap(cc_list_t *self, cc_list_t *others);



int cc_list_is_empty(cc_list_t *self);
cc_size_t cc_list_size(cc_list_t *self);

int cc_list_to_cc_array(cc_list_t *self, cc_array_t **result);

int cc_list_print(cc_list_t *self, int direction, cc_debug_print_fn_t cc_debug_print);

struct cc_list_iterator {
    cc_iter_i_t *interface;
    cc_list_t *list;
    cc_list_node_t *cursor;
    cc_size_t index;
    int direction;
};
typedef struct cc_list_iterator cc_list_iterator_t;

int cc_list_iter_init(cc_list_iterator_t *self, cc_list_t *list, int direction);
int cc_list_iter_new(cc_list_iterator_t **self, cc_list_t *list, int direction);
int cc_list_iter_delete(cc_list_iterator_t *self);
int cc_list_iter_next(cc_list_iterator_t *self, void **item, cc_size_t *index);



#endif // __CC_LIST_H__
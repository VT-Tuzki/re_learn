#include <stdio.h>
#include "cc_common.h"
#include "cc_list.h"
#include <string.h>
#include "cc_mem.h"
#include <assert.h>

/*
    cc_list_new
    cc_list_size
    cc_list_is_empty
    cc_list_insert_tail
    cc_list_insert_head
    cc_list_remove_head
    cc_list_remove_tail
    cc_list_concat
    cc_list_destroy
*/


struct list_node {
    cc_size_t number;
    char   name[20];
};
typedef struct list_node list_node_t;

int print_list_node_data(list_node_t *data)
{
    printf("num: %ld name: %s\n",data->number,data->name);
    return 0;
}

#define LOOP_LEN  3

int main()
{
    cc_list_t *test_list_a = NULL;
    cc_list_t *test_list_b = NULL;
    cc_size_t now_len = 0;

    assert(cc_list_new(&test_list_a) == ERR_CC_LIST_OK);
    assert(cc_list_new(&test_list_b) == ERR_CC_LIST_OK);

    now_len = cc_list_size(test_list_a);
    assert(now_len == 0);
    assert(cc_list_is_empty(test_list_a) == 1);


    for(cc_size_t i = 0; i < LOOP_LEN; i++) {
        list_node_t *temp_list_node = cc_malloc(sizeof(*temp_list_node));
        assert(temp_list_node != NULL);
        temp_list_node->number = i;
        strcpy(temp_list_node->name,"insert_head");
        assert(cc_list_insert_tail(test_list_a, temp_list_node)==ERR_CC_LIST_OK);
        now_len = cc_list_size(test_list_a);
        assert(now_len == i+1);
    }
    /*
        0-1-2

    */
    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN);
    assert(cc_list_print(test_list_a, 1, (cc_debug_print_fn_t) print_list_node_data) == ERR_CC_LIST_OK);

    for(cc_size_t i = 0; i < LOOP_LEN; i++) {
        list_node_t *temp_list_node = cc_malloc(sizeof(*temp_list_node));
        assert(temp_list_node != NULL);
        temp_list_node->number = i;
        strcpy(temp_list_node->name,"insert_tail");
        assert(cc_list_insert_head(test_list_a, temp_list_node)==ERR_CC_LIST_OK);

        now_len = cc_list_size(test_list_a);
        assert(now_len == LOOP_LEN+i+1);
    }
    /*
        2-1-0-0-1-2

    */
    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN*2);
    assert(cc_list_print(test_list_a, 1, (cc_debug_print_fn_t) print_list_node_data) == ERR_CC_LIST_OK);


    list_node_t *temp = NULL;

    assert(cc_list_get_head(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    print_list_node_data(temp);
    assert(temp->number == 2);

    assert(cc_list_get_tail(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    assert(temp->number == 2);

    assert(cc_list_remove_head(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    assert(temp->number == 2);
    now_len = cc_list_size(test_list_a);
    cc_free(temp);
    assert(now_len == LOOP_LEN*2 - 1);
    /*
        1-0-0-1-2

    */

    assert(cc_list_remove_tail(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    assert(temp->number == 2);
        cc_free(temp);

    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN*2 - 2);
    /*
        1-0-0-1

    */
    assert(cc_list_remove_head(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    cc_free(temp);
    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN*2 - 3);

    assert(cc_list_remove_head(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    cc_free(temp);
    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN*2 - 4);

    assert(cc_list_remove_tail(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    cc_free(temp);
    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN*2 - 5);

    assert(cc_list_remove_tail(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    cc_free(temp);
    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN*2 - 6);

    assert(cc_list_remove_tail(test_list_a, (void **) &(temp)) == ERR_CC_LIST_EMPTY);

    for(cc_size_t i = 0; i < LOOP_LEN; i++) {
        list_node_t *temp_list_node = cc_malloc(sizeof(*temp_list_node));
        assert(temp_list_node != NULL);
        temp_list_node->number = i;
        strcpy(temp_list_node->name,"insert_head_a");
        assert(cc_list_insert_tail(test_list_a, temp_list_node)==ERR_CC_LIST_OK);
        now_len = cc_list_size(test_list_a);
        assert(now_len == i+1);
    }
    assert(cc_list_print(test_list_a, 1, (cc_debug_print_fn_t) print_list_node_data) == ERR_CC_LIST_OK);


    for(cc_size_t i = 0; i < LOOP_LEN; i++) {
        list_node_t *temp_list_node = cc_malloc(sizeof(*temp_list_node));
        assert(temp_list_node != NULL);
        temp_list_node->number = i;
        strcpy(temp_list_node->name,"insert_head_b");
        assert(cc_list_insert_tail(test_list_b, temp_list_node)==ERR_CC_LIST_OK);
        now_len = cc_list_size(test_list_b);
        assert(now_len == i+1);
    }
    assert(cc_list_print(test_list_b, 1, (cc_debug_print_fn_t) print_list_node_data) == ERR_CC_LIST_OK);

    printf("concat a\n");
    assert(cc_list_concat(test_list_a, test_list_b) == ERR_CC_LIST_OK);
    assert(cc_list_print(test_list_a, 1, (cc_debug_print_fn_t) print_list_node_data) == ERR_CC_LIST_OK);
    printf("reverse print a\n");
    assert(cc_list_print(test_list_a, 0, (cc_debug_print_fn_t) print_list_node_data) == ERR_CC_LIST_OK);

    printf("concat b\n");
    assert(cc_list_print(test_list_b, 1, (cc_debug_print_fn_t) print_list_node_data) == ERR_CC_LIST_OK);


    printf("iter\n");
    cc_list_iterator_t *iter;
    list_node_t **temp_list_node_iter ;
    cc_size_t i = 0;
    assert(cc_list_iter_new(&iter, test_list_a, 1) == ERR_CC_COMMON_OK);

    while(cc_iter_next(iter, &temp_list_node_iter, &i) != ERR_CC_ITER_END) {
        printf("index:%ld  ",i);
        print_list_node_data(*temp_list_node_iter);
    }
    cc_list_iter_delete(iter);
    printf("distory\n");

    assert(cc_list_destroy(test_list_a, (cc_delete_fn_t) cc_free) == ERR_CC_LIST_OK);

    assert(cc_list_destroy(test_list_b, (cc_delete_fn_t) cc_free) == ERR_CC_LIST_OK);


    return 0;
}
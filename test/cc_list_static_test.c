#include <stdio.h>
#include "core/cc_common.h"
#include "ds/list/cc_list.h"
#include <string.h>
#include "core/cc_mem.h"
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

    assert(cc_list_new(&test_list_a, (cc_delete_fn_t) adapter_free) == ERR_CC_LIST_OK);
    assert(cc_list_new(&test_list_b, (cc_delete_fn_t) adapter_free) == ERR_CC_LIST_OK);

    now_len = cc_list_size(test_list_a);
    assert(now_len == 0);
    assert(cc_list_is_empty(test_list_a) == 1);


    for(cc_size_t i = 0; i < LOOP_LEN; i++) {
        list_node_t *temp_list_node = malloc(sizeof(*temp_list_node));
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
        list_node_t *temp_list_node = malloc(sizeof(*temp_list_node));
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
    adapter_free(temp);
    assert(now_len == LOOP_LEN*2 - 1);
    /*
        1-0-0-1-2

    */

    assert(cc_list_remove_tail(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    assert(temp->number == 2);
        adapter_free(temp);

    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN*2 - 2);
    /*
        1-0-0-1

    */
    assert(cc_list_remove_head(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    adapter_free(temp);
    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN*2 - 3);

    assert(cc_list_remove_head(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    adapter_free(temp);
    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN*2 - 4);

    assert(cc_list_remove_tail(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    adapter_free(temp);
    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN*2 - 5);

    assert(cc_list_remove_tail(test_list_a, (void **) &(temp)) == ERR_CC_LIST_OK);
    adapter_free(temp);
    now_len = cc_list_size(test_list_a);
    assert(now_len == LOOP_LEN*2 - 6);

    assert(cc_list_remove_tail(test_list_a, (void **) &(temp)) == ERR_CC_LIST_EMPTY);

    for(cc_size_t i = 0; i < LOOP_LEN; i++) {
        list_node_t *temp_list_node = malloc(sizeof(*temp_list_node));
        assert(temp_list_node != NULL);
        temp_list_node->number = i;
        strcpy(temp_list_node->name,"insert_head_a");
        assert(cc_list_insert_tail(test_list_a, temp_list_node)==ERR_CC_LIST_OK);
        now_len = cc_list_size(test_list_a);
        assert(now_len == i+1);
    }
    assert(cc_list_print(test_list_a, 1, (cc_debug_print_fn_t) print_list_node_data) == ERR_CC_LIST_OK);


    for(cc_size_t i = 0; i < LOOP_LEN; i++) {
        list_node_t *temp_list_node = malloc(sizeof(*temp_list_node));
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

    assert(cc_list_destroy(test_list_a) == ERR_CC_LIST_OK);

    assert(cc_list_destroy(test_list_b) == ERR_CC_LIST_OK);

    // Test cc_list_node_remove_node
    printf("=== Testing cc_list_node_remove_node ===\n");
    cc_list_t *test_list_node_remove;
    assert(cc_list_new(&test_list_node_remove, adapter_free) == ERR_CC_LIST_OK);

    // Insert test nodes
    for(cc_size_t i = 0; i < 5; i++) {
        list_node_t *temp_node = malloc(sizeof(*temp_node));
        assert(temp_node != NULL);
        temp_node->number = i;
        strcpy(temp_node->name, "test_remove");
        assert(cc_list_insert_tail(test_list_node_remove, temp_node) == ERR_CC_LIST_OK);
    }

    // The list should have 5 elements now
    assert(cc_list_size(test_list_node_remove) == 5);
    printf("List before node removal:\n");
    assert(cc_list_print(test_list_node_remove, 1, (cc_debug_print_fn_t) print_list_node_data) == ERR_CC_LIST_OK);

    // Get the 3rd node (node with number 2)
    cc_list_node_t *target_node = test_list_node_remove->root.next->next->next;
    void *target_data = target_node->data;
    void *target_data_get = NULL;
    // Get the nodes before and after the target
    cc_list_node_t *prev_node = target_node->prev;
    cc_list_node_t *next_node = target_node->next;

    // Remove the node using cc_list_node_remove_node
    assert(cc_list_remove_node(test_list_node_remove, target_node, &target_data_get) == ERR_CC_LIST_OK);
     assert(target_data_get == target_data);
    // Free the node and its data
    adapter_free(target_data);

    // The list should have 4 elements now
    assert(cc_list_size(test_list_node_remove) == 4);
    printf("List after node removal:\n");
    assert(cc_list_print(test_list_node_remove, 1, (cc_debug_print_fn_t) print_list_node_data) == ERR_CC_LIST_OK);

    // Verify the links are correct
    assert(prev_node->next == next_node);
    assert(next_node->prev == prev_node);

    // Clean up
    assert(cc_list_destroy(test_list_node_remove) == ERR_CC_COMMON_OK);
    printf("cc_list_node_remove_node test completed successfully!\n");

    return 0;
}
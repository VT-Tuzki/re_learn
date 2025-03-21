#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cc_common.h"
#include "cc_stdint.h"
#include "cc_list.h"
#include "cc_list_pool.h"
#include "cc_dbg.h"
#include "cc_run_time.h"
#include "cc_mem.h"

/* Test data structure, similar to cc_list_1_static_test.c */
struct test_data {
    cc_size_t number;
    char *name;
};
typedef struct test_data test_data_t;

#define POOL_CAPACITY 100
#define NAME_LEN 20
#define TEST_LIST_SIZE 50

/* Static node pool declaration */
CC_LIST_NODE_POOL_STATIC_DECLARE(static_list_node_pool,  POOL_CAPACITY);

/* Helper functions */
int free_test_data(test_data_t *self);
test_data_t *create_test_data(cc_size_t number, const char *name);
int print_test_data(void *data);

/* Test functions for node pool */
void test_node_pool_lifecycle();
void test_node_pool_acquire_release();
void test_node_pool_capacity();

/* Test functions for list with pool */
void test_list_pool_lifecycle();
void test_list_pool_insert_operations();
void test_list_pool_remove_operations();
void test_list_pool_edge_operations();

/* Performance comparison */
void test_performance_comparison();

int main() {
    printf("===== Starting cc_list_pool tests =====\n\n");
    CC_LIST_NODE_POOL_CONNECT(static_list_node_pool);
    /* Node pool tests */
    test_node_pool_lifecycle();
    test_node_pool_acquire_release();
    test_node_pool_capacity();

    /* List with pool tests */
    test_list_pool_lifecycle();
    test_list_pool_insert_operations();
    test_list_pool_remove_operations();
    test_list_pool_edge_operations();

    /* Performance comparison */
    test_performance_comparison();

    printf("\n===== All cc_list_pool tests passed! =====\n");
    return 0;
}

/* Helper function implementation */
int free_test_data(test_data_t *self) {
    if (self == NULL) return ERR_CC_COMMON_INVALID_ARG;

    free(self->name);
    self->name = NULL;
    free(self);
    return ERR_CC_COMMON_OK;
}

test_data_t *create_test_data(cc_size_t number, const char *name) {
    test_data_t *data = malloc(sizeof(test_data_t));
    if (!data) return NULL;

    data->number = number;
    data->name = malloc(NAME_LEN);
    if (!data->name) {
        free(data);
        return NULL;
    }

    strncpy(data->name, name, NAME_LEN - 1);
    data->name[NAME_LEN - 1] = '\0';

    return data;
}

int print_test_data(void *data) {
    test_data_t *test_data = *(test_data_t **)data;
    printf("Number: %zu, Name: %s\n", test_data->number, test_data->name);
    return ERR_CC_COMMON_OK;
}

/* Node pool test implementation */
void test_node_pool_lifecycle() {
    printf("Testing node pool lifecycle...\n");

    cc_list_node_pool_t pool;

    // Test initialization
    assert(cc_list_node_pool_init(&pool, 50) == ERR_CC_COMMON_OK);

    // Test static initialization
    assert(cc_list_node_pool_static_init(&static_list_node_pool) == ERR_CC_COMMON_OK);

    // Test destruction
    assert(cc_list_node_pool_destroy(&pool) == ERR_CC_COMMON_OK);

    printf("Node pool lifecycle test passed!\n\n");
}

void test_node_pool_acquire_release() {
    printf("Testing node pool acquire/release...\n");

    cc_list_node_t *nodes[10];

    // Acquire nodes
    for (int i = 0; i < 10; i++) {
        assert(cc_list_node_pool_acquire(&static_list_node_pool, &nodes[i]) == ERR_CC_COMMON_OK);
        assert(nodes[i] != NULL);

        // Initialize the node data
        nodes[i]->data = create_test_data(i, "test");
    }

    // Release nodes
    for (int i = 0; i < 10; i++) {
        void *data = nodes[i]->data;
        assert(cc_list_node_pool_release(&static_list_node_pool, nodes[i]) == ERR_CC_COMMON_OK);
        free_test_data(data);
    }

    printf("Node pool acquire/release test passed!\n\n");
}

void test_node_pool_capacity() {
    printf("Testing node pool capacity...\n");

    cc_list_node_pool_t small_pool;
    assert(cc_list_node_pool_init(&small_pool, 5) == ERR_CC_COMMON_OK);

    cc_list_node_t *nodes[10];
    int res;

    // Fill the pool
    for (int i = 0; i < 5; i++) {
        res = cc_list_node_pool_acquire(&small_pool, &nodes[i]);
        assert(res == ERR_CC_COMMON_OK);
    }

    // Should fail when pool is empty
    cc_list_node_t *overflow_node;
    res = cc_list_node_pool_acquire(&small_pool, &overflow_node);
    assert(res == ERR_CC_POOL_EMPTY);

    // Release and reacquire
    assert(cc_list_node_pool_release(&small_pool, nodes[0]) == ERR_CC_COMMON_OK);
    assert(cc_list_node_pool_acquire(&small_pool, &nodes[0]) == ERR_CC_COMMON_OK);

    // Cleanup
    for (int i = 0; i < 5; i++) {
        cc_list_node_pool_release(&small_pool, nodes[i]);
    }
    cc_list_node_pool_destroy(&small_pool);

    printf("Node pool capacity test passed!\n\n");
}

/* List with pool test implementation */
void test_list_pool_lifecycle() {
    printf("Testing list with pool lifecycle...\n");

    cc_list_with_pool_t *list;

    // Test creation
    assert(cc_list_new_with_pool(&list, &static_list_node_pool, (cc_delete_fn_t)free_test_data) == ERR_CC_COMMON_OK);

    // Verify initialization
    assert(list->pool == &static_list_node_pool);
    assert(list->list.root.next == &list->list.root);
    assert(list->list.root.prev == &list->list.root);
    assert(list->list.root.size == 0);

    // Test destruction
    assert(cc_list_destroy_with_pool(list) == ERR_CC_COMMON_OK);

    printf("List with pool lifecycle test passed!\n\n");
}

void test_list_pool_insert_operations() {
    printf("Testing list with pool insert operations...\n");

    cc_list_with_pool_t *list;
    assert(cc_list_new_with_pool(&list, &static_list_node_pool, (cc_delete_fn_t)free_test_data) == ERR_CC_COMMON_OK);

    // Insert data
    test_data_t *data1 = create_test_data(1, "first");
    test_data_t *data2 = create_test_data(2, "second");
    test_data_t *data3 = create_test_data(3, "third");

    // Test head insertion
    assert(cc_list_insert_head_with_pool(list, data1) == ERR_CC_COMMON_OK);
    assert(list->list.root.size == 1);
    assert((list->list.root.next->data) == data1);

    // Test tail insertion
    assert(cc_list_insert_tail_with_pool(list, data2) == ERR_CC_COMMON_OK);
    assert(list->list.root.size == 2);
    assert((list->list.root.prev->data) == data2);

    // Test mixed insertion
    assert(cc_list_insert_head_with_pool(list, data3) == ERR_CC_COMMON_OK);
    assert(list->list.root.size == 3);
    assert((list->list.root.next->data) == data3);

    // Verify order: data3 -> data1 -> data2
    void *curr_data;
    assert(cc_list_get_head(&list->list, &curr_data) == ERR_CC_COMMON_OK);
    assert(curr_data == data3);

    cc_list_destroy_with_pool(list);

    printf("List with pool insert operations test passed!\n\n");
}

void test_list_pool_remove_operations() {
    printf("Testing list with pool remove operations...\n");

    cc_list_with_pool_t *list;
    assert(cc_list_new_with_pool(&list, &static_list_node_pool, (cc_delete_fn_t)free_test_data) == ERR_CC_COMMON_OK);

    // Setup test data
    test_data_t *data1 = create_test_data(1, "first");
    test_data_t *data2 = create_test_data(2, "second");
    test_data_t *data3 = create_test_data(3, "third");

    // Insert data
    assert(cc_list_insert_head_with_pool(list, data1) == ERR_CC_COMMON_OK);
    assert(cc_list_insert_tail_with_pool(list, data2) == ERR_CC_COMMON_OK);
    assert(cc_list_insert_tail_with_pool(list, data3) == ERR_CC_COMMON_OK);
    assert(list->list.root.size == 3);

    // Test remove head
    void *removed_data;
    assert(cc_list_remove_head_with_pool(list, &removed_data) == ERR_CC_COMMON_OK);
    assert((test_data_t*)removed_data == data1);
    free_test_data((test_data_t*)removed_data);
    assert(list->list.root.size == 2);

    // Test remove tail
    assert(cc_list_remove_tail_with_pool(list, &removed_data) == ERR_CC_COMMON_OK);
    assert((test_data_t*)removed_data == data3);
    free_test_data((test_data_t*)removed_data);
    assert(list->list.root.size == 1);

    // Verify remaining data
    assert(cc_list_get_head(&list->list, &removed_data) == ERR_CC_COMMON_OK);
    assert((test_data_t*)removed_data == data2);

    // Clean up
    cc_list_destroy_with_pool(list);

    printf("List with pool remove operations test passed!\n\n");
}

void test_list_pool_edge_operations() {
    printf("Testing list with pool edge cases...\n");
    cc_list_with_pool_t *list;
    assert(cc_list_new_with_pool(&list, &static_list_node_pool, (cc_delete_fn_t)free_test_data) == ERR_CC_COMMON_OK);

    // Test operations on empty list
    void *data;
    assert(cc_list_remove_head_with_pool(list, &data) == ERR_CC_LIST_EMPTY);
    assert(cc_list_remove_tail_with_pool(list, &data) == ERR_CC_LIST_EMPTY);

    // Test with NULL parameters
    assert(cc_list_insert_head_with_pool(NULL, NULL) == ERR_CC_COMMON_INVALID_ARG);
    assert(cc_list_insert_tail_with_pool(NULL, NULL) == ERR_CC_COMMON_INVALID_ARG);
    assert(cc_list_remove_head_with_pool(NULL, &data) == ERR_CC_COMMON_INVALID_ARG);
    assert(cc_list_remove_tail_with_pool(NULL, &data) == ERR_CC_COMMON_INVALID_ARG);

    // Clean up
    cc_list_destroy_with_pool(list);

    printf("List with pool edge cases test passed!\n\n");
}

void test_performance_comparison() {
    printf("Testing performance comparison between regular list and list with pool...\n");

    const int TEST_ITERATIONS = 10000;

    // Setup pools and lists
    cc_list_node_pool_t perf_pool;
    assert(cc_list_node_pool_init(&perf_pool, TEST_ITERATIONS) == ERR_CC_COMMON_OK);

    cc_list_with_pool_t *pooled_list;
    assert(cc_list_new_with_pool(&pooled_list, &perf_pool, NULL) == ERR_CC_COMMON_OK);

    cc_list_t *regular_list;
    assert(cc_list_new(&regular_list, NULL) == ERR_CC_COMMON_OK);



    // Test regular list insert performance
    macro_timer_start(usuall_list)
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        int *data = malloc(sizeof(int));
        *data = i;
        cc_list_insert_tail(regular_list, data);
    }
    macro_timer_end(usuall_list)

    macro_timer_start(pool_list)
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        int *data = malloc(sizeof(int));
        *data = i;
        cc_list_insert_tail_with_pool(pooled_list, data);
    }
    macro_timer_end(pool_list)

    // Clean up
    // Free data in regular list
    void *data;
    while (cc_list_remove_head(regular_list, &data) == ERR_CC_COMMON_OK) {
        free(data);
    }
    cc_list_destroy(regular_list);

    // Free data in pooled list
    while (cc_list_remove_head_with_pool(pooled_list, &data) == ERR_CC_COMMON_OK) {
        free(data);
    }
    cc_list_destroy_with_pool(pooled_list);
    cc_list_node_pool_destroy(&perf_pool);

    printf("Performance comparison completed!\n\n");
}

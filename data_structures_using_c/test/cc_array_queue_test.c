#include "cc_array_queue.h"
#include "cc_list_stack.h"
#include "cc_common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_QUEUE_CAPACITY 4

static void test_basic_operations(void) {
    cc_array_queue_t *queue;
    int result;

    // 初始化队列（存储栈指针）
    result = cc_array_queue_new(&queue, TEST_QUEUE_CAPACITY, sizeof(cc_list_stack_t*));
    assert(result == ERR_CC_COMMON_OK);

    // 验证初始状态
    assert(cc_array_queue_is_empty(queue) == ERR_CC_QUEUE_EMPTY);
    assert(cc_array_queue_size(queue) == 0);

    // 创建并压入3个栈
    for (int i = 0; i < 3; i++) {
        cc_list_stack_t *stack;
        cc_list_stack_new(&stack);

        // 在栈中压入测试数据
        int *data = malloc(sizeof(int));
        *data = i;
        cc_list_stack_push(stack, data);

        // 入队操作
        result = cc_array_queue_enqueue(queue, &stack);
        assert(result == ERR_CC_COMMON_OK);
        assert(cc_array_queue_size(queue) == (cc_size_t)(i + 1));
    }

    // 验证非空状态
    assert(cc_array_queue_is_empty(queue) == ERR_CC_COMMON_OK);
    assert(cc_array_queue_size(queue) == 3);

    // 出队并验证数据
    for (int i = 0; i < 3; i++) {
        cc_list_stack_t *stack;
        result = cc_array_queue_dequeue(queue, (void**)&stack);
        assert(result == ERR_CC_COMMON_OK);

        // 验证栈数据
        int *data;
        cc_list_stack_pop(stack, (void**)&data);
        assert(*data == i);
        free(data);
        cc_list_stack_delete(stack, (cc_delete_fn_t)free);
    }

    // 验证最终状态
    assert(cc_array_queue_is_empty(queue) == ERR_CC_QUEUE_EMPTY);
    cc_array_queue_delete(queue, NULL);
}

static void test_boundary_conditions(void) {
    cc_array_queue_t *queue;
    cc_array_queue_new(&queue, TEST_QUEUE_CAPACITY, sizeof(cc_list_stack_t*));

    // 测试空队列出队
    void *temp;
    assert(cc_array_queue_dequeue(queue, &temp) == ERR_CC_QUEUE_EMPTY);

    // 填满队列
    for (int i = 0; i < TEST_QUEUE_CAPACITY; i++) {
        cc_list_stack_t *stack;
        cc_list_stack_new(&stack);
        assert(cc_array_queue_enqueue(queue, &stack) == ERR_CC_COMMON_OK);
    }

    // 测试满队列入队
    cc_list_stack_t *overflow_stack;
    cc_list_stack_new(&overflow_stack);
    assert(cc_array_queue_enqueue(queue, &overflow_stack) == ERR_CC_QUEUE_IS_FULL);
    cc_list_stack_delete(overflow_stack, NULL);

    // 清空队列
    while (cc_array_queue_size(queue) > 0) {
        cc_list_stack_t *stack;
        cc_array_queue_dequeue(queue, (void**)&stack);
        cc_list_stack_delete(stack, NULL);
    }

    cc_array_queue_delete(queue, NULL);
}

static void test_circular_behavior(void) {
    cc_array_queue_t *queue;
    cc_array_queue_new(&queue, TEST_QUEUE_CAPACITY, sizeof(cc_list_stack_t*));

    // 测试循环特性
    for (int cycle = 0; cycle < 2; cycle++) {
        // 填充队列
        for (int i = 0; i < TEST_QUEUE_CAPACITY; i++) {
            cc_list_stack_t *stack;
            cc_list_stack_new(&stack);
            assert(cc_array_queue_enqueue(queue, &stack) == ERR_CC_COMMON_OK);
        }

        // 清空队列
        for (int i = 0; i < TEST_QUEUE_CAPACITY; i++) {
            cc_list_stack_t *stack;
            assert(cc_array_queue_dequeue(queue, (void**)&stack) == ERR_CC_COMMON_OK);
            cc_list_stack_delete(stack, NULL);
        }
    }

    cc_array_queue_delete(queue, NULL);
}

int main(void) {
    test_basic_operations();
    test_boundary_conditions();
    test_circular_behavior();

    printf("All cc_array_queue tests passed!\n");
    return 0;
}

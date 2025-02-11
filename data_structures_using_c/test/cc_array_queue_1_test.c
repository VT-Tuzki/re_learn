#include "cc_array_queue.h"
#include "cc_list_stack.h"
#include <assert.h>
#include <stdio.h>

// 测试队列初始化与销毁
void test_queue_init_and_delete() {
    cc_array_queue_t *queue = NULL;
    int res;

    // 测试正常创建
    res = cc_array_queue_new(&queue, 5, sizeof(cc_list_stack_t*));
    assert(res == ERR_CC_COMMON_OK);
    assert(queue != NULL);
    assert(cc_array_queue_size(queue) == 0);

    // 测试销毁
    res = cc_array_queue_delete(queue, NULL);
    assert(res == ERR_CC_COMMON_OK);
}

// 测试基本入队出队操作
void test_enqueue_dequeue() {
    cc_array_queue_t *queue = NULL;
    cc_list_stack_t *stack = NULL;
    void *dequeued = NULL;
    int res;

    cc_array_queue_new(&queue, 3, sizeof(cc_list_stack_t*));

    // 创建测试栈
    res = cc_list_stack_new(&stack);
    assert(res == ERR_CC_LIST_OK);

    // 测试空队列状态
    assert(cc_array_queue_is_empty(queue) == ERR_CC_QUEUE_EMPTY);

    // 入队测试
    res = cc_array_queue_enqueue(queue, &stack);
    assert(res == ERR_CC_COMMON_OK);
    assert(cc_array_queue_size(queue) == 1);

    // 出队验证
    res = cc_array_queue_dequeue(queue, &dequeued);
    assert(res == ERR_CC_COMMON_OK);
    assert(dequeued == stack);
    assert(cc_array_queue_size(queue) == 0);

    // 清理
    cc_list_stack_delete(stack, NULL);
    cc_array_queue_delete(queue, NULL);
}

// 测试队列满/边界条件
void test_full_behavior() {
    cc_array_queue_t *queue = NULL;
    cc_list_stack_t *stacks[3] = {NULL};
    int res;

    cc_array_queue_new(&queue, 2, sizeof(cc_list_stack_t*));

    // 创建2个栈填满队列
    for (int i = 0; i < 2; i++) {
        res = cc_list_stack_new(&stacks[i]);
        assert(res == ERR_CC_LIST_OK);
        res = cc_array_queue_enqueue(queue, &stacks[i]);
        assert(res == ERR_CC_COMMON_OK);
    }

    // 测试队列满
    assert(cc_array_queue_is_full(queue) == ERR_CC_QUEUE_IS_FULL);

    // 测试超额入队
    res = cc_list_stack_new(&stacks[2]);
    assert(res == ERR_CC_LIST_OK);
    res = cc_array_queue_enqueue(queue, &stacks[2]);
    assert(res == ERR_CC_QUEUE_IS_FULL);

    // 清理
    for (int i = 0; i < 3; i++) {
        if (stacks[i]) cc_list_stack_delete(stacks[i], NULL);
    }
    cc_array_queue_delete(queue, NULL);
}

// 测试复杂结构操作
void test_stack_in_queue() {
    cc_array_queue_t *queue = NULL;
    cc_list_stack_t *stack = NULL;
    int values[] = {10, 20, 30};
    void *dequeued_stack = NULL;
    int res;

    cc_array_queue_new(&queue, 1, sizeof(cc_list_stack_t*));

    // 创建并初始化栈
    res = cc_list_stack_new(&stack);
    assert(res == ERR_CC_LIST_OK);
    for (int i = 0; i < 3; i++) {
        res = cc_list_stack_push(stack, &values[i]);
        assert(res == ERR_CC_LIST_OK);
    }

    // 入队栈
    res = cc_array_queue_enqueue(queue, &stack);
    assert(res == ERR_CC_COMMON_OK);

    // 出队并验证栈内容
    res = cc_array_queue_dequeue(queue, &dequeued_stack);
    assert(res == ERR_CC_COMMON_OK);

    int *peek_value;
    res = cc_list_stack_peek((cc_list_stack_t*)dequeued_stack, (void**)&peek_value);
    assert(res == ERR_CC_COMMON_OK); // 应为ERR_CC_LIST_OK？需要检查实现
    assert(*peek_value == 30);

    // 清理
    cc_list_stack_delete((cc_list_stack_t*)dequeued_stack, NULL);
    cc_array_queue_delete(queue, NULL);
}

int main() {
    test_queue_init_and_delete();
    printf("Queue init/delete test passed.\n");

    test_enqueue_dequeue();
    printf("Basic enqueue/dequeue test passed.\n");

    test_full_behavior();
    printf("Queue full behavior test passed.\n");

    test_stack_in_queue();
    printf("Complex structure test passed.\n");

    printf("All tests passed successfully!\n");
    return 0;
}
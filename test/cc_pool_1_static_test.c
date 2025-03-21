// cc_pool_1_static_test.c
#include "cc_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cc_dbg.h"
#include "cc_stdint.h"
// 测试结构体定义
typedef struct {
    char name[32];
    int age;
    double score;
} student_t;
#define STATIC_POOL_SIZE 1000
// 声明静态对象池（测试新方案）
CC_POOL_STATIC_DECLARE(static_stu_pool, student_t, STATIC_POOL_SIZE, NULL);

// 边界测试结构体
typedef struct {
    char data[512];
} large_data_t;

void test_static_pool() {
    printf("===== Static Pool Test =====\n");
    int res = ERR_CC_COMMON_OK;
    // 测试重复初始化保护
    res = cc_pool_static_init(&static_stu_pool);
    check_res_ok(res, "cc_pool_static_init errr");
    assert(cc_pool_static_init(&static_stu_pool) == ERR_CC_COMMON_FAIL);
    cc_size_t available_size = 0;
    cc_size_t capacity = 0;
    uint8_t is_full = 0;
    assert(cc_pool_get_capacity(&static_stu_pool, &capacity) == ERR_CC_COMMON_OK);
    assert(capacity == STATIC_POOL_SIZE);
    assert(cc_pool_get_available_size(&static_stu_pool, &available_size) == ERR_CC_COMMON_OK);
    assert(available_size == STATIC_POOL_SIZE);
    assert(cc_pool_is_full(&static_stu_pool, &is_full) == ERR_CC_COMMON_OK);
    assert(is_full == 0);
    // 批量分配测试
    student_t* students[STATIC_POOL_SIZE] = {0};
    for (int i = 0; i < STATIC_POOL_SIZE; i++) {

        res = cc_pool_acquire(&static_stu_pool, (void**)&students[i]);
        check_res_ok(res, "cc_pool_acquire errr");
        snprintf(students[i]->name, 32, "Student%d", i);
        students[i]->age = i % 100 + 18;
        students[i]->score = i % 100;
        assert(cc_pool_get_available_size(&static_stu_pool, &available_size) == ERR_CC_COMMON_OK);
        // printf("%d, %ld\n",i,available_size);
        assert(available_size == (cc_size_t)(STATIC_POOL_SIZE - i - 1));
    }
    assert(cc_pool_get_available_size(&static_stu_pool, &available_size) == ERR_CC_COMMON_OK);
    assert(available_size == 0);
    assert(cc_pool_is_full(&static_stu_pool, &is_full) == ERR_CC_COMMON_OK);
    assert(is_full == 1);
    // 测试池耗尽
    student_t* overflow = NULL;
    res = cc_pool_acquire(&static_stu_pool, (void**)&overflow);
    check((res == ERR_CC_POOL_EMPTY), "res:%d",res);
    // 批量释放测试
    for (int i = 0; i < STATIC_POOL_SIZE/2; i++) {
        cc_pool_release_ptr(&static_stu_pool, students[i]);
    }

    // 交错分配测试
    for (int i = 0; i < STATIC_POOL_SIZE/2; i++) {
        assert(cc_pool_acquire(&static_stu_pool, (void**)&students[i]) == ERR_CC_COMMON_OK);
    }

    // 完全释放
    for (int i = 0; i < STATIC_POOL_SIZE; i++) {
        cc_pool_release_ptr(&static_stu_pool, students[i]);
    }

    printf("Static pool test passed!\n\n");
    return;
error:
    exit(1);
}
#define CAPACITY 500

void test_dynamic_pool() {
    printf("===== Dynamic Pool Test =====\n");
    cc_pool_t dyn_pool;

    // 初始化动态池
    assert(cc_pool_init(&dyn_pool, sizeof(large_data_t), CAPACITY, NULL) == ERR_CC_COMMON_OK);

    // 压力测试
    large_data_t* data_ptrs[CAPACITY] = {0};
    for (int i = 0; i < CAPACITY; i++) {
        assert(cc_pool_acquire(&dyn_pool, (void**)&data_ptrs[i]) == ERR_CC_COMMON_OK);
        memset(data_ptrs[i]->data, i % 256, sizeof(data_ptrs[i]->data));
    }

    // 验证数据完整性
    for (int i = 0; i < CAPACITY; i++) {
        for (cc_size_t j = 0; j < sizeof(data_ptrs[i]->data); j++) {
            assert(data_ptrs[i]->data[j] == (char)(i % 256));
        }
    }

    // 异常释放测试（重复释放）
    cc_pool_release_ptr(&dyn_pool, data_ptrs[0]);
    cc_pool_release_ptr(&dyn_pool, data_ptrs[0]);  // 应该安全处理

    // 部分释放后重新分配
    for (int i = 100; i < 200; i++) {
        cc_pool_release_ptr(&dyn_pool, data_ptrs[i]);
    }
    for (int i = 100; i < 200; i++) {
        assert(cc_pool_acquire(&dyn_pool, (void**)&data_ptrs[i]) == ERR_CC_COMMON_OK);
    }

    // 销毁池测试
    assert(cc_pool_destroy(&dyn_pool) == ERR_CC_COMMON_OK);
    printf("Dynamic pool test passed!\n\n");
}

void test_edge_cases() {
    printf("===== Edge Case Test =====\n");
    // 测试0容量池（应该失败）
    cc_pool_t zero_pool;
    assert(cc_pool_init(&zero_pool, sizeof(int), 0, NULL) == ERR_CC_COMMON_INVALID_ARG);

    // 测试超大元素
    typedef struct { char data[1024*1024]; } huge_t;
    cc_pool_t huge_pool;
    assert(cc_pool_init(&huge_pool, sizeof(huge_t), 10, NULL) == ERR_CC_COMMON_OK);
    huge_t* h = NULL;
    assert(cc_pool_acquire(&huge_pool, (void**)&h) == ERR_CC_COMMON_OK);
    assert(h != NULL);
    assert(cc_pool_destroy(&huge_pool) == ERR_CC_COMMON_OK);

    printf("Edge case test passed!\n\n");
}

int main() {
    test_static_pool();
    test_dynamic_pool();
    test_edge_cases();
    printf("All tests completed successfully!\n");
    return 0;
}

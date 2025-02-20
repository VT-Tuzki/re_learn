#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "core/cc_common.h"
#include "core/cc_stdint.h"
#include "core/cc_dbg.h"
#include "core/cc_mem.h"
#include "core/cc_run_time.h"

#include "ds/array/cc_array.h"
#include "ds/list/cc_list.h"
#include "ds/list/cc_list_sort.h"



void test_bubble_sort_basic();


int main()
{
    /*sort*/
    test_bubble_sort_basic();


    return 0;
}

static int copy_int_data(const void *src, void **dest)
{
    int *new_data = malloc(sizeof(int));
    if (!new_data) return ERR_CC_COMMON_MEM_ERR;
    memcpy(new_data, src, sizeof(int));
    *dest = new_data;
    return ERR_CC_COMMON_OK;
}


int print_int(void *a) {
    int val_a = *(int *)a;
    printf("%d,",val_a);
    return 0;
}

int cmp_int(void *a, void *b) {
    int val_a = *(int *)a;
    int val_b = *(int *)b;
    return val_a - val_b;
}

#define ARR_LEN 15
void test_bubble_sort_basic()
{
    int res = ERR_CC_ARRAY_OK;
    cc_list_t *list;
    cc_list_new(&list, cc_free);
    for(int i = 0; i < ARR_LEN; i++){
        int *num = malloc(sizeof(int));
        *num = rand() % 1000;
        cc_list_insert_tail(list, num);
    }
    // cc_list_print(list, 1, (cc_debug_print_fn_t)print_int);
    // printf("\n---\n");

    cc_list_t *traditional_list, *adaptive_list, *get_end_list;

    res = cc_list_copy(&traditional_list, list, (cc_copy_data_fn_t)copy_int_data);
    check_res_ok(res, "copy err");

    res = cc_list_copy(&adaptive_list, list, (cc_copy_data_fn_t)copy_int_data);
    check_res_ok(res, "copy err");

    res = cc_list_copy(&get_end_list, list, (cc_copy_data_fn_t)copy_int_data);
    check_res_ok(res, "copy err");

    macro_timer_start(traditional_sort_time);
    res = cc_list_sort_bubble(traditional_list, cmp_int, 1);
    macro_timer_end(traditional_sort_time);
    check_res_ok(res, "sort err");

    macro_timer_start(adaptive_sort_time);
    res = cc_list_sort_bubble(adaptive_list, cmp_int, 2);
    check_res_ok(res, "sort err");
    macro_timer_end(adaptive_sort_time);

    macro_timer_start(get_end_sort_time);
    res = cc_list_sort_bubble(get_end_list, cmp_int, 3);
    macro_timer_end(get_end_sort_time);
    check_res_ok(res, "sort errr");

    cc_size_t sort_ok_traditional,sort_ok_adaptive, sort_ok_get_end;
    // printf("\n----------\n");

    // cc_list_print(traditional_list, 1, (cc_debug_print_fn_t)print_int);
    // printf("\n");
macro_timer_start(check_traditional_time);
    res = cc_list_sort_check(traditional_list, cmp_int, &sort_ok_traditional);
macro_timer_end(check_traditional_time);
    check((sort_ok_traditional == 1), "traditional_sort err %ld",sort_ok_traditional);
    check_res_ok(res, "traditional_list_check err");


    // cc_list_print(adaptive_list, 1, (cc_debug_print_fn_t)print_int);
    // printf("\n");
macro_timer_start(check_adaptive_time);
    res = cc_list_sort_check(adaptive_list, cmp_int, &sort_ok_adaptive);
macro_timer_end(check_adaptive_time);
    check((sort_ok_adaptive == 1), "adaptive_sort err %ld",sort_ok_adaptive);
    check_res_ok(res, "adaptive_list_check err");

    // cc_list_print(get_end_list, 1, (cc_debug_print_fn_t)print_int);
    // printf("\n");
macro_timer_start(check_get_end_time);
    res = cc_list_sort_check(get_end_list, cmp_int, &sort_ok_get_end);
macro_timer_end(check_get_end_time);
    check((sort_ok_get_end == 1), "get_end_sort err %ld",sort_ok_get_end);
    check_res_ok(res, "get_end_list_check err");
    cc_list_destroy(list);
    cc_list_destroy(traditional_list);
    cc_list_destroy(adaptive_list);
    cc_list_destroy(get_end_list);

    return;
error:
    exit(1);
}
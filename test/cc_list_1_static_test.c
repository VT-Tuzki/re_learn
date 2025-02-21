#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "cc_common.h"
#include "cc_list_sort.h"
#include "cc_stdint.h"
#include "ds/array/cc_array.h"
#include "ds/list/cc_list.h"

#include "core/cc_dbg.h"
#include "core/cc_run_time.h"
#include "core/cc_mem.h"
/*
 node data
    list->data = &array;
    array->data = &test_data_t;
*/

struct test_data {
    cc_size_t number;
    char  *name;
};
typedef struct test_data test_data_t;
#define ARRAY_LEN 10
#define NAME_LEN 20
#define ARRAY_LIST_LEN 5
char array_name_str[ARRAY_LIST_LEN][NAME_LEN] = {
    "array0","array1","array2","array3","array4",
};


/* others */
int free_test_data(test_data_t *self);
int get_array(cc_array_t **self, char *str);
int print_array(cc_array_t *self);
int print_int(void *a);
/*  basic */
//new init
void test_lifecycle();
void test_insert_operations();
void test_remove_operations();
void test_edge_operations();

/*  copy */
void test_copy_empty_list();
void test_copy_single_node();
void test_copy_multiple_nodes();
void test_copy_error_handling();

/*  contant */
void test_concat_empty_left();
void test_concat_normal_list();
void test_concat_error_handling();

void test_concat_cmp_normal_list();
void test_concat_cmp_error_right_0_1();
void test_concat_cmp_error_left_0_1();

/* split */
void test_split_empty_left();
void test_split_normal_list();
void test_split_error_handling();
void test_split_middle_empty_error_handling();
void test_split_middle_normal_list();

int main(){
/*  basic */

/* new init */
//     test_lifecycle();
//     test_insert_operations();
//     test_remove_operations();
//     test_edge_operations();

// /*  copy */
//     test_copy_empty_list();
//     test_copy_single_node();
//     test_copy_multiple_nodes();
//     test_copy_error_handling();

// /*  contant */
//     test_concat_empty_left();
//     test_concat_normal_list();
//     test_concat_error_handling();

//     test_concat_cmp_normal_list();
//     test_concat_cmp_error_right_0_1();
//     test_concat_cmp_error_left_0_1();
// /* split */
//     test_split_empty_left();
//     test_split_normal_list();
//     test_split_error_handling();

//     test_split_middle_empty_error_handling();
//     test_split_middle_normal_list();
    return 0;
}

/* others */

static int free_test_data_wrapper(void *elem)
{
    test_data_t **ptr = (test_data_t **)elem;
    return free_test_data(*ptr);
}

int free_test_data(test_data_t *self)
{
    if(self == NULL) return ERR_CC_COMMON_INVALID_ARG;
    //log_info("free: ptr:%p name_ptr:%p, namestr:%s,i:%ld,len:%ld",self,self->name,self->name,self->number, strlen(self->name));

    free(self->name);
    self->name = NULL;
    free(self);
    return ERR_CC_COMMON_OK;
}
int free_test_array_data(void *elem)
{
    if(elem == NULL) return ERR_CC_COMMON_INVALID_ARG;
    cc_array_t **ptr = (cc_array_t **)elem;
    return cc_array_delete(*ptr);
}

int get_array(cc_array_t **self, char *str)
{
    int res = ERR_CC_ARRAY_OK;
    cc_array_t *tmp = NULL;
    test_data_t *temp_data = NULL;
    res = cc_array_new(&tmp, ARRAY_LEN, sizeof(test_data_t *), (cc_delete_fn_t)free_test_data_wrapper);
    for(int i = 0; i < ARRAY_LEN; i++) {
        temp_data = malloc(sizeof(*temp_data));
        check(temp_data,"malloc temp_data error\n");
        temp_data->name = malloc(NAME_LEN);
        check(temp_data,"malloc temp_data->name error\n");
        strcpy(temp_data->name, str);
        temp_data->number = i;
        //log_info("ptr:%p nameptr:%p namestr:%s,i:%ld,len:%ld",temp_data,temp_data->name, temp_data->name,temp_data->number, strlen(temp_data->name));
        cc_array_set(tmp, i, &temp_data);
    }

    *self = tmp;
    return ERR_CC_COMMON_OK;

fail2:
    cc_array_delete(tmp);
fail1:
    return res;
error:
    exit(1);
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

/*  basic */
//new init
void test_lifecycle()
{
    cc_list_t *list;
    assert(cc_list_new(&list, cc_free) == ERR_CC_COMMON_OK);
    assert(list->root.next == &list->root);
    assert(list->root.prev == &list->root);
    assert(list->root.size == 0);
    cc_list_destroy(list);
}
//insert
void test_insert_operations()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *list;
    assert(cc_list_new(&list, (cc_delete_fn_t)free_test_array_data) == ERR_CC_COMMON_OK);
    cc_array_t *array1,*array2;
    res = get_array(&array1,"array1");
    check((res == ERR_CC_COMMON_OK), "array1 create fail");
    res = get_array(&array2,"array2");
    check((res == ERR_CC_COMMON_OK), "array2 create fail");

    res = cc_list_insert_head(list, &array1);
    check((res == ERR_CC_COMMON_OK), "array1 insert list ok");
    check((list->root.next->data == &array1), "head add node error");
    check((list->root.prev->data == &array1), "head add node error");
    check((list->root.size == 1), "cc_list_insert_head size not change");
    res = cc_list_insert_tail(list, &array2);
    check((res == ERR_CC_COMMON_OK), "array2 insert list ok");
    check((list->root.next->data == &array1), "head add node error");
    check((list->root.prev->data == &array2), "head add node error");
    check((list->root.size == 2), "size not add");

    check((list->root.next->next == list->root.prev), "list next cycle not connect");
    check((list->root.prev->prev == list->root.next), "list prev cycle not connect");
    res = cc_list_print(list,1,NULL);
    res = cc_list_destroy(list);
    check((res == ERR_CC_COMMON_OK), "list destroy err");
    return ;
error:
    exit(1);
}
//remove
void test_remove_operations()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *list;
    assert(cc_list_new(&list, (cc_delete_fn_t)free_test_array_data) == ERR_CC_COMMON_OK);
    cc_array_t *array1,*array2;
    res = get_array(&array1,"array1");
    check((res == ERR_CC_COMMON_OK), "array1 create fail");
    res = get_array(&array2,"array2");
    check((res == ERR_CC_COMMON_OK), "array2 create fail");
    //log_info("array1:%p,&array1:%p,array2:%p,&array2:%p",array1,&array1,array2,&array2);
    res = cc_list_insert_head(list, &array1);
    check((res == ERR_CC_COMMON_OK), "array1 insert list error");
    res = cc_list_insert_tail(list, &array2);
    check((res == ERR_CC_COMMON_OK), "array2 insert list error");

    void *removed_data;
    res = cc_list_remove_head(list, (void **)&removed_data);
    check((res == ERR_CC_COMMON_OK), "cc_list_remove_head remove array 1 list error");
    check((removed_data == &array1), "cc_list_remove_head data err removedptr:%p,arrayptr:%p,array:%p",removed_data,&array1,array1);
    check((list->root.size == 1), "cc_list_remove_head remove size not change");
    check((list->root.next->data == &array2), "cc_list_remove_head cycle not connect");

    res = cc_list_remove_tail(list, &removed_data);
    check((res == ERR_CC_COMMON_OK), "cc_list_remove_head remove array2 list error");
    check((removed_data == &array2), "cc_list_remove_head data err");
    check((list->root.size == 0), "cc_list_remove_head remove size not change");
    check((list->root.next == &list->root), "cc_list_remove_head cycle not connect");

    res = cc_list_destroy(list);
    check((res == ERR_CC_COMMON_OK), "list destroy err");

    res = cc_array_delete(array1);
    check((res == ERR_CC_COMMON_OK), "array1 destroy err");
    res = cc_array_delete(array2);
    check((res == ERR_CC_COMMON_OK), "array2 destroy err");
    return;
error:
    exit(1);
}
//edge state
void test_edge_operations()
{
    cc_list_t list;
    cc_list_init(&list, cc_free);
    assert(cc_list_remove_head(&list, NULL) == ERR_CC_LIST_EMPTY);
    assert(cc_list_remove_tail(&list, NULL) == ERR_CC_LIST_EMPTY);
    assert(cc_list_remove_head(NULL, NULL) == ERR_CC_LIST_INVALID_ARG);
    assert(cc_list_remove_tail(NULL, NULL) == ERR_CC_LIST_INVALID_ARG);

}


/*  copy */

static int copy_int_data(const void *src, void **dest)
{
    int *new_data = malloc(sizeof(int));
    if (!new_data) return ERR_CC_COMMON_MEM_ERR;
    memcpy(new_data, src, sizeof(int));
    *dest = new_data;
    return ERR_CC_COMMON_OK;
}

// static int copy_test_data_t(const void *src, void **dest)
// {
//     const test_data_t *src_test_data = *(const test_data_t **) src;
//     test_data_t *new_test_data = malloc(sizeof(test_data_t));
//     new_test_data->number = src_test_data->number;
//     new_test_data->name = malloc(NAME_LEN);
//     strcpy(new_test_data->name, src_test_data->name);
//     *dest = new_test_data;
// }

// static int copy_array_data(const void *src, void **dest)
// {
//     int *new_data = malloc(sizeof(cc_array_t *));
//     if (!new_data) return ERR_CC_COMMON_MEM_ERR;


//     *dest = new_data;
//     return ERR_CC_COMMON_OK;
// }

void test_copy_empty_list()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *list;
    res = cc_list_new(&list, cc_free);
    check((res == ERR_CC_COMMON_OK), "new fail");
    cc_list_t *copy_list;
    res = cc_list_copy(&copy_list, list, (cc_copy_data_fn_t)copy_int_data);
    check((res == ERR_CC_COMMON_OK), "copy fail %d",res);

    check(cc_list_size(copy_list) == 0, "copy size is not same");
    check((copy_list->root.next == &copy_list->root), "copy list cycle not connect");
    check((copy_list->root.prev == &copy_list->root), "copy list cycle not connect");

    cc_list_destroy(copy_list);
    check((res == ERR_CC_COMMON_OK), "destroy fail");
    cc_list_destroy(list);
    check((res == ERR_CC_COMMON_OK), "destroy fail");

    return;
error:
    exit(1);
}

void test_copy_single_node()
{

    int res = ERR_CC_COMMON_OK;
    cc_list_t *list;
    res = cc_list_new(&list, cc_free);
    check((res == ERR_CC_COMMON_OK), "new fail");

    int *data = malloc(sizeof(int));
    *data = 1;
    res = cc_list_insert_head(list, data);
    check((res == ERR_CC_COMMON_OK), "insert fail");

    cc_list_t *copy_list;
    res = cc_list_copy(&copy_list, list, (cc_copy_data_fn_t)copy_int_data);
    check((res == ERR_CC_COMMON_OK), "copy fail");

    int *copy_data;
    res = cc_list_get_head(copy_list, (void**)&copy_data);
    check((res == ERR_CC_COMMON_OK), "cc_list_get_head fail");
    check((*copy_data == *data), "copy data:%d, data:%d",*copy_data,*data);
    check(cc_list_size(copy_list) == 1, "copy size is not same");

    cc_list_destroy(copy_list);
    check((res == ERR_CC_COMMON_OK), "destroy fail");
    cc_list_destroy(list);
    check((res == ERR_CC_COMMON_OK), "destroy fail");

    return;
error:
    exit(1);
}

static int get_list_array_data_struct_str(cc_list_node_t *list_node, void **result)
{
    cc_array_t *old_array = list_node->data;
    test_data_t *old_test_data = (test_data_t *)old_array->data;
    char *old_str = old_test_data->name;
    *result = old_str;
    return ERR_CC_COMMON_OK;
}

void test_copy_multiple_nodes()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *old_list;

    res = cc_list_new(&old_list, (cc_delete_fn_t)cc_free);
    check((res == ERR_CC_COMMON_OK), "new error");

    for(int i = 0; i < ARRAY_LIST_LEN; i++) {
        int *data = malloc(sizeof(int));
        *data = i;
        res = cc_list_insert_head(old_list, data);
        check((res == ERR_CC_COMMON_OK), "insert fail");
    }

    cc_list_t *new_list;
    res = cc_list_copy(&new_list, old_list, (cc_copy_data_fn_t) copy_int_data);
    check((res == ERR_CC_COMMON_OK), "copy fail %d",res);

    check((cc_list_size(new_list) == cc_list_size(old_list)),"copy list error old:%ld new:%ld",cc_list_size(old_list), cc_list_size(new_list));

    cc_list_node_t *old_list_node = old_list->root.next;
    cc_list_node_t *new_list_node = new_list->root.next;

    while(old_list_node != &old_list->root) {
        int *old_int,*new_int;
        old_int = (old_list_node->data);
        new_int = (new_list_node->data);
        check((*old_int == *new_int), "str error,old:%d,new:%d", *old_int, *new_int);
        check((old_int != new_int), "str ptr error old%p, new%p", old_int, new_int);
        old_list_node = old_list_node->next;
        new_list_node = new_list_node->next;
    }

    check((new_list_node == &new_list->root ), "new list cycle not connect");

    cc_list_destroy(new_list);
    cc_list_destroy(old_list);
    return;
error:
    exit(1);
}
void test_copy_error_handling()
{
    int res = cc_list_copy(NULL, NULL, NULL);
    check((res == ERR_CC_COMMON_INVALID_ARG), "ARG check err");

    cc_list_t vaild_list;
    cc_list_t *new_list;
    res = cc_list_copy(&new_list, &vaild_list, NULL);
    check((res != ERR_CC_COMMON_OK), "ARG check err");

    return;
error:
    exit(1);
}


/*  contant */

void test_concat_empty_left()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *left;
    cc_list_new(&left, cc_free);

    cc_list_t *right;
    cc_list_new(&right, cc_free);
    int *b1 = malloc(sizeof(int)); *b1 = 5;
    cc_list_insert_tail(right, b1);
    int *b2 = malloc(sizeof(int)); *b2 = 6;
    cc_list_insert_tail(right, b2);

    res = cc_list_concat(left, right);
    check((res == ERR_CC_LIST_OK), "concat err %d", res);

    check((left->root.size == 2), "size error %ld",left->root.size);
    check((left->root.next->data == b1), "concat add node err");
    check((left->root.next->next->data == b2), "concat add node err");

    // right应被清空
    check((right->root.size == 0), "right list error");
    check((right->root.next == &right->root), "right list cycle not connect");
    check((right->root.prev == &right->root), "right list cycle not connect");

    cc_list_destroy(left);
    cc_list_destroy(right);
    return;
error:
    exit(1);
}
void test_concat_normal_list()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *left;
    cc_list_new(&left, cc_free);
    int *a1 = malloc(sizeof(int)); *a1 = 1;
    cc_list_insert_tail(left, a1);
    int *a2 = malloc(sizeof(int)); *a2 = 2;
    cc_list_insert_tail(left, a2);

    cc_list_t *right;
    cc_list_new(&right, cc_free);
    int *b1 = malloc(sizeof(int)); *b1 = 3;
    cc_list_insert_tail(right, b1);
    int *b2 = malloc(sizeof(int)); *b2 = 4;
    cc_list_insert_tail(right, b2);

    res = cc_list_concat(left, right);
    check((res == ERR_CC_LIST_OK), "concat err %d", res);

    check((left->root.size == 4), "size error %ld",left->root.size);
    check((left->root.next->data == a1), "concat add node err");
    check((left->root.next->next->data == a2), "concat add node err");
    check((left->root.next->next->next->data == b1), "concat add node err");
    check((left->root.next->next->next->next->data == b2), "concat add node err");
    // right应被清空
    check((right->root.size == 0), "right list error");
    check((right->root.next == &right->root), "right list cycle not connect");
    check((right->root.prev == &right->root), "right list cycle not connect");

    cc_list_destroy(left);
    cc_list_destroy(right);
    return;
error:
    exit(1);
}

void test_concat_error_handling()
{
    cc_list_t valid_list;
    check((cc_list_concat(&valid_list, NULL) == ERR_CC_COMMON_INVALID_ARG), "arg err");

    check((cc_list_concat(NULL, &valid_list) == ERR_CC_COMMON_INVALID_ARG), "arg err");
    return;
error:
    exit(1);
}

void test_concat_cmp_normal_list()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *left;
    cc_list_new(&left, cc_free);
    int *a1 = malloc(sizeof(int)); *a1 = 2;
    cc_list_insert_tail(left, a1);
    int *a2 = malloc(sizeof(int)); *a2 = 3;
    cc_list_insert_tail(left, a2);

    cc_list_t *right;
    cc_list_new(&right, cc_free);
    int *b1 = malloc(sizeof(int)); *b1 = 1;
    cc_list_insert_tail(right, b1);
    int *b2 = malloc(sizeof(int)); *b2 = 4;
    cc_list_insert_tail(right, b2);

    res = cc_list_concat_by_cmp(left, right, cmp_int);
    check((res == ERR_CC_LIST_OK), "concat err %d", res);

    check((left->root.size == 4), "size error %ld",left->root.size);
    check((left->root.next->data == b1), "concat add node err");
    check((left->root.next->next->data == a1), "concat add node err");
    cc_size_t dir = 0;
    res= cc_list_sort_check(left, cmp_int, &dir);
    check((dir == 1), "err sort %ld", dir);
    res = cc_list_print(left,1,print_int);
    printf("\n");
    // right应被清空
    check((right->root.size == 0), "right list error");
    check((right->root.next == &right->root), "right list cycle not connect");
    check((right->root.prev == &right->root), "right list cycle not connect");

    cc_list_destroy(left);
    cc_list_destroy(right);
    return;
error:
    exit(1);
}

void test_concat_cmp_error_right_0_1()
{
    cc_size_t dir = 0;

    int res = ERR_CC_COMMON_OK;
    cc_list_t *left;
    cc_list_new(&left, cc_free);
    int *a1 = malloc(sizeof(int)); *a1 = 2;
    cc_list_insert_tail(left, a1);
    int *a2 = malloc(sizeof(int)); *a2 = 3;
    cc_list_insert_tail(left, a2);

    cc_list_t *right;
    cc_list_new(&right, cc_free);
    // left: 2 3 right : null
    res = cc_list_concat_by_cmp(left, right, cmp_int);
    check((res == ERR_CC_LIST_OK), "concat err %d", res);
    check((left->root.size == 2), "size error %ld",left->root.size);
    check((left->root.next->data == a1), "concat add node err");
    check((left->root.next->next->data == a2), "concat add node err");
    check((left->root.next->next->next == &left->root), "concat add node err");
    // right应被清空
    check((right->root.size == 0), "right list error");
    check((right->root.next == &right->root), "right list cycle not connect");
    check((right->root.prev == &right->root), "right list cycle not connect");

    int *b1 = malloc(sizeof(int)); *b1 = 1;
    cc_list_insert_tail(right, b1);
    // left: 2 3 right : 1
    res = cc_list_concat_by_cmp(left, right, cmp_int);
    check((res == ERR_CC_LIST_OK), "concat err %d", res);
    check((left->root.size == 3), "size error %ld",left->root.size);
    check((left->root.next->data == b1), "concat add node err");
    check((left->root.next->next->data == a1), "concat add node err");
    check((left->root.next->next->next->data == a2), "concat add node err");
    check((left->root.next->next->next->next == &left->root), "concat add node err");
    // right应被清空
    check((right->root.size == 0), "right list error");
    check((right->root.next == &right->root), "right list cycle not connect");
    check((right->root.prev == &right->root), "right list cycle not connect");
    res= cc_list_sort_check(left, cmp_int, &dir);
    check((dir == 1), "err sort %ld", dir);
    //  1 2 3
    // right应被清空
    check((right->root.size == 0), "right list error");
    check((right->root.next == &right->root), "right list cycle not connect");
    check((right->root.prev == &right->root), "right list cycle not connect");

    cc_list_destroy(left);
    cc_list_destroy(right);
    return;
error:
    exit(1);
}

void test_concat_cmp_error_left_0_1()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *left;
    cc_list_new(&left, cc_free);

    cc_list_t *right;
    cc_list_new(&right, cc_free);
    int *b1 = malloc(sizeof(int)); *b1 = 1;
    cc_list_insert_tail(right, b1);
    int *b2 = malloc(sizeof(int)); *b2 = 4;
    cc_list_insert_tail(right, b2);
    //left : null right 1 4

    res = cc_list_concat_by_cmp(left, right, cmp_int);
    check((res == ERR_CC_LIST_OK), "concat err %d", res);

    check((left->root.size == 2), "size error %ld",left->root.size);
    check((left->root.next->data == b1), "concat add node err");
    check((left->root.next->next->data == b2), "concat add node err");
    check((left->root.next->next->next == &left->root), "concat add node err");
    // right应被清空
    check((right->root.size == 0), "right list error, %ld",right->root.size);
    check((right->root.next == &right->root), "right list cycle not connect");
    check((right->root.prev == &right->root), "right list cycle not connect");

    cc_list_destroy(left);
    left = NULL;
    res = cc_list_new(&left, cc_free);
    check_res_ok(res, "new err");
    b1 = malloc(sizeof(int)); *b1 = 1;
    cc_list_insert_tail(right, b1);
    b2 = malloc(sizeof(int)); *b2 = 4;
    cc_list_insert_tail(right, b2);

    int *a1 = malloc(sizeof(int)); *a1 = 2;
    cc_list_insert_tail(left, a1);
    //left : 2 right 1 4
    res = cc_list_concat_by_cmp(left, right, cmp_int);
    check((res == ERR_CC_LIST_OK), "concat err %d", res);

    check((left->root.size == 3), "size error %ld",left->root.size);
    check((left->root.next->data == b1), "concat add node err");
    check((left->root.next->next->data == a1), "concat add node err");
    check((left->root.next->next->next->data == b2), "concat add node err");
    check((left->root.next->next->next->next == &left->root), "concat add node err");
    // right应被清空
    check((right->root.size == 0), "right list error");
    check((right->root.next == &right->root), "right list cycle not connect");
    check((right->root.prev == &right->root), "right list cycle not connect");

    cc_size_t dir = 0;
    res= cc_list_sort_check(left, cmp_int, &dir);
    check((dir == 1), "err sort %ld", dir);
    // res = cc_list_print(left,1,print_int);
    // printf("\n");

    cc_list_destroy(left);
    cc_list_destroy(right);
    return;
error:
    exit(1);
}

/* split */

int check_data_need_split(void *data)
{
    if(data == NULL) return ERR_CC_COMMON_INVALID_ARG;
    int value = *(int*)data;
    return (value > 1) ? ERR_CC_COMMON_OK : ERR_CC_COMMON_FAIL;
}

void test_split_empty_left()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *old_list;
    cc_list_new(&old_list, cc_free);
    check((cc_list_size(old_list) == 0), "insert err");

    cc_list_t *new_list = NULL;
    res = cc_list_split(&new_list, old_list, (cc_check_fn_t) check_data_need_split);
    check((res == ERR_CC_COMMON_OK), "split fail %d", res);

    check((cc_list_size(old_list) == 0), "insert err");
    check((cc_list_size(new_list) == 0), "insert err");

    cc_list_destroy(old_list);
    cc_list_destroy(new_list);

    return;
error:
    exit(1);
}
void test_split_normal_list()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *old_list;
    cc_list_new(&old_list, cc_free);
    for(int i = 0; i < ARRAY_LEN; i++) {
        int *data = malloc(sizeof(*data));
        *data = i;
        cc_list_insert_tail(old_list, data);
    }
    check((cc_list_size(old_list) == ARRAY_LEN), "insert err");

    cc_list_t *new_list = NULL;
    res = cc_list_split(&new_list, old_list, (cc_check_fn_t) check_data_need_split);
    check((res == ERR_CC_COMMON_OK), "split fail %d", res);

    check((cc_list_size(old_list) == 2), "insert err");
    check((cc_list_size(new_list) == ARRAY_LEN - 2), "insert err");

    cc_list_destroy(old_list);
    cc_list_destroy(new_list);
    return;
error:
    exit(1);
}

void test_split_error_handling()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t list;
    cc_list_t *new_list = NULL;
    res = cc_list_split(NULL, &list, (cc_check_fn_t) check_data_need_split);
    check((res == ERR_CC_COMMON_INVALID_ARG), "arg err");
    res = cc_list_split(&new_list, NULL, (cc_check_fn_t) check_data_need_split);
    check((res == ERR_CC_COMMON_INVALID_ARG), "arg err");
    res = cc_list_split(&new_list, &list, NULL);
    check((res == ERR_CC_COMMON_INVALID_ARG), "arg err");
    return;
error:
    exit(1);
}

void test_split_middle_empty_error_handling()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t list;
    cc_list_t *new_list = NULL;
    res = cc_list_split_middle(NULL, &list);
    check((res == ERR_CC_COMMON_INVALID_ARG), "arg err %d",res);
    res = cc_list_split_middle(&new_list, NULL);
    check((res == ERR_CC_COMMON_INVALID_ARG), "arg err %d",res);
    cc_list_new(&new_list, NULL);
    res = cc_list_split_middle(&new_list, &list);
    check((res == ERR_CC_COMMON_INVALID_ARG), "arg err %d",res);
    cc_list_destroy(new_list);
    return;
error:
    exit(1);
}
void test_split_middle_normal_list()
{
    int res = ERR_CC_COMMON_OK;
    cc_list_t *old_list;
    cc_list_new(&old_list, cc_free);
    for(int i = 0; i < ARRAY_LEN; i++) {
        int *data = malloc(sizeof(*data));
        *data = i;
        cc_list_insert_tail(old_list, data);
    }
    check((cc_list_size(old_list) == ARRAY_LEN), "insert err");

    cc_list_t *new_list = NULL;
    res = cc_list_split_middle(&new_list, old_list);
    check((res == ERR_CC_COMMON_OK), "split fail %d", res);

    check((cc_list_size(old_list) == (ARRAY_LEN /2)),
        "old_list size err %ld ,%d",cc_list_size(old_list), (ARRAY_LEN /2));
    check((cc_list_size(new_list) == (ARRAY_LEN - (ARRAY_LEN /2))),
        "new_list size err %ld ,%d", cc_list_size(new_list),(ARRAY_LEN - (ARRAY_LEN /2)));

    res = cc_list_concat(old_list, new_list);
    check_res_ok(res, "concat err");
    cc_list_destroy(new_list);
    new_list = NULL;
    int *data = malloc(sizeof(*data));
    *data = ARRAY_LEN;
    cc_list_insert_tail(old_list, data);

    res = cc_list_split_middle(&new_list, old_list);
    check((res == ERR_CC_COMMON_OK), "split fail %d", res);

    cc_size_t now_len = ARRAY_LEN + 1;
    check((cc_list_size(old_list) == (now_len / 2)),
        "old_list size err %ld ,%ld",cc_list_size(old_list), (now_len / 2));
    check((cc_list_size(new_list) == (now_len - (now_len /2))),
        "new_list size err %ld ,%ld", cc_list_size(new_list),(now_len - (now_len / 2)));

    cc_list_destroy(old_list);
    cc_list_destroy(new_list);
    return;
error:
    exit(1);
}
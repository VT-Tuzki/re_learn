/*
 * @Author: shucai 1374643996@qq.com
 * @Date: 2025-01-13 10:08:47
 * @LastEditors: shucai 1374643996@qq.com
 * @LastEditTime: 2025-01-13 12:30:39
 * @FilePath: /simple_progream_with_lib_src/include/simple_array.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CC_ARRAY_H__
#define __CC_ARRAY_H__

#include "cc_stdint.h"
#include "cc_common.h"
#include "cc_iter.h"
struct cc_array {
    unsigned char *data;
    cc_size_t elem_nums;
    cc_size_t elem_size;
};
typedef struct cc_array cc_array_t;

typedef enum {
    ERR_CC_ARRAY_OK = ERR_CC_COMMON_OK,
    ERR_CC_ARRAY_INVALID_ARG = ERR_CC_COMMON_INVALID_ARG,
    ERR_CC_ARRAY_MEM_ERR = ERR_CC_COMMON_MEM_ERR,
    ERR_CC_ARRAY_EMPTY,
} cc_array_err;


int cc_array_init(cc_array_t *self, unsigned char *data, cc_size_t elem_nums, cc_size_t elem_size);
/***
 * @description:
 * @param {cc_array} *
 * @param {cc_size_t} elem_nums
 * @param {cc_size_t} elem_size
 * @return {*}
 */
int cc_array_new(cc_array_t **self, cc_size_t elem_nums, cc_size_t elem_size);

int cc_array_delete(struct cc_array *self, cc_delete_fn_t remove_fn);

#define CC_ARRAY_STATIC_DECLARE(name, elem_num, elem_size) \
    unsigned char name##_heap[(elem_num) * (elem_size)] = {0}; \
    cc_array_t name = { \
        .data = name##_heap, \
        .elem_nums = (elem_num), \
        .elem_size = (elem_size) \
    }

int cc_array_get_unsafe(cc_array_t *self, cc_size_t index, void *result);
int cc_array_get_ref(cc_array_t *self, cc_size_t index, void **ref);
int cc_array_get(cc_array_t *self, cc_size_t index, void *result);

int cc_array_set_unsafe(cc_array_t *self, cc_size_t index, void *value);
int cc_array_set(cc_array_t *self, cc_size_t index, void *value);

int cc_array_is_vaild_index(cc_array_t *self, cc_size_t index);
int cc_array_cmp(cc_array_t *self, cc_cmp_fn_t cmp, cc_size_t i, cc_size_t j);
int cc_array_swap(cc_array_t *self, cc_size_t i, cc_size_t j);
int cc_array_reverse(cc_array_t *self, cc_size_t start, cc_size_t end);

int cc_array_copy_index(cc_array_t *array_a, cc_array_t * array_b, cc_size_t index_a, cc_size_t index_b);

int cc_array_resize(cc_array_t *self, cc_size_t new_elem_nums);



struct cc_array_iter {
    cc_iter_i_t *iterator;
    cc_array_t *data;
    cc_size_t cursor;
};
typedef struct cc_array_iter cc_array_iter_t;

int cc_array_iterator_init(cc_array_iter_t *self, cc_array_t *data);
int cc_array_iterator_new(cc_array_iter_t **self, cc_array_t *data);
int cc_array_iterator_delete(cc_array_iter_t *self);

int cc_array_iterator_next(cc_array_iter_t *self,void **item, cc_size_t *index);


#endif
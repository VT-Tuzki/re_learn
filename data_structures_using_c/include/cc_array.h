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

#include <stddef.h>
#include "cc_common.h"

struct cc_array {
    unsigned char *data;
    size_t elem_nums;
    size_t elem_size;
};


int cc_array_init(struct cc_array *self, unsigned char *data, size_t elem_nums, size_t elem_size);
/***
 * @description:
 * @param {cc_array} *
 * @param {size_t} elem_nums
 * @param {size_t} elem_size
 * @return {*}
 */
int cc_array_new(struct cc_array **self, size_t elem_nums, size_t elem_size);

int cc_array_delete(struct cc_array *self);


int cc_array_get_unsafe(struct cc_array *self, size_t index, void *result);
int cc_array_get_ref(struct cc_array *self, size_t index, void **ref);
int cc_array_get(struct cc_array *self, size_t index, void *result);

int cc_array_set_unsafe(struct cc_array *self, size_t index, void *value);
int cc_array_set(struct cc_array *self, size_t index, void *value);

int cc_array_is_vaild_index(struct cc_array *self, size_t index);
int cc_array_cmp(struct cc_array *self, cc_cmp_fn_t cmp, size_t i, size_t j);
int cc_array_swap(struct cc_array *self, size_t i, size_t j);
int cc_array_reverse(struct cc_array *self, size_t start, size_t end);


int cc_array_copy_index(struct cc_array *array_a, struct cc_array * array_b, size_t index_a, size_t index_b);

// struct cc_array_iter {
//     struct cc_iter_i *iterator;
//     struct cc_array *data;
//     size_t cursor;
// };

// int cc_array_iterator_init(struct cc_array_iter *self, struct cc_array *data);
// int cc_array_iterator_next(struct cc_array_iter *self,void **item, size_t *index);


#endif
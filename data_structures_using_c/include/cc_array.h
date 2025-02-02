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
typedef struct cc_array cc_array_t;

typedef enum {
    ERR_CC_ARRAY_OK = ERR_CC_COMMON_OK,
    ERR_CC_ARRAY_INVALID_ARG = ERR_CC_COMMON_INVALID_ARG,
    ERR_CC_ARRAY_MEM_ERR = ERR_CC_COMMON_MEM_ERR,
    ERR_CC_ARRAY_EMPTY,
} cc_array_err;


int cc_array_init(cc_array_t *self, unsigned char *data, size_t elem_nums, size_t elem_size);
/***
 * @description:
 * @param {cc_array} *
 * @param {size_t} elem_nums
 * @param {size_t} elem_size
 * @return {*}
 */
int cc_array_new(cc_array_t **self, size_t elem_nums, size_t elem_size);

int cc_array_delete(cc_array_t *self);


int cc_array_get_unsafe(cc_array_t *self, size_t index, void *result);
int cc_array_get_ref(cc_array_t *self, size_t index, void **ref);
int cc_array_get(cc_array_t *self, size_t index, void *result);

int cc_array_set_unsafe(cc_array_t *self, size_t index, void *value);
int cc_array_set(cc_array_t *self, size_t index, void *value);

int cc_array_is_vaild_index(cc_array_t *self, size_t index);
int cc_array_cmp(cc_array_t *self, cc_cmp_fn_t cmp, size_t i, size_t j);
int cc_array_swap(cc_array_t *self, size_t i, size_t j);
int cc_array_reverse(cc_array_t *self, size_t start, size_t end);

int cc_array_copy_index(cc_array_t *array_a, cc_array_t * array_b, size_t index_a, size_t index_b);

int cc_array_resize(cc_array_t *self, size_t new_elem_nums);



// struct cc_array_iter {
//     struct cc_iter_i *iterator;
//     cc_array_t *data;
//     size_t cursor;
// };

// int cc_array_iterator_init(struct cc_array_iter *self, cc_array_t *data);
// int cc_array_iterator_next(struct cc_array_iter *self,void **item, size_t *index);


#endif
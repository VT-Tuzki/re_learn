/*
 * @Author: shucai 1374643996@qq.com
 * @Date: 2025-01-13 10:08:47
 * @LastEditors: shucai 1374643996@qq.com
 * @LastEditTime: 2025-01-13 12:30:39
 * @FilePath: /simple_progream_with_lib_src/include/simple_array.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __SIMPLE_ARRAY_H__
#define __SIMPLE_ARRAY_H__

#include <stddef.h>

struct s_array{
    unsigned char *data;
    size_t elem_nums;
    size_t elem_size;
};


int cc_array_init(struct s_array *self, unsigned char *data, size_t elem_nums, size_t elem_size);


/***
 * @description:
 * @param {s_array} *
 * @param {size_t} elem_nums
 * @param {size_t} elem_size
 * @return {*}
 */
int cc_array_new(struct s_array **self, size_t elem_nums, size_t elem_size);

int cc_array_delete(struct s_array *self);


int cc_array_get_unsafe(struct s_array *self, size_t index, void *result);
int cc_array_get_ref(struct s_array *self, size_t index, void **ref);
int cc_array_get(struct s_array *self, size_t index, void *result);

int cc_array_set_unsafe(struct s_array *self, size_t index, void *value);
int cc_array_set(struct s_array *self, size_t index, void *value);

#endif
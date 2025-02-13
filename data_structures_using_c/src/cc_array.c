#include "cc_array.h"
#include "cc_common.h"
#include "cc_mem.h"
#include <string.h>
#include "cc_atomic.h"

int cc_array_init(struct cc_array *self, unsigned char *data, cc_size_t elem_nums, cc_size_t elem_size)
{
    if(data == NULL || elem_size == 0) return ERR_CC_COMMON_INVALID_ARG;
    self->data = data;
    self->elem_nums = elem_nums;
    self->elem_size = elem_size;
    return ERR_CC_ARRAY_OK;
}


int cc_array_new(struct cc_array **self, cc_size_t elem_nums, cc_size_t elem_size)
{
    int res = ERR_CC_ARRAY_OK;
    struct cc_array *tmp = NULL;
    unsigned char* data = NULL;

    tmp = cc_malloc(sizeof(struct cc_array));
    if(tmp == NULL) {
        res = ERR_CC_ARRAY_MEM_ERR;
        goto fail1;
    }

    data = cc_malloc(elem_size * elem_nums);
    if(data == NULL) {
        res = ERR_CC_ARRAY_MEM_ERR;
        goto fail2;
    }

    res = cc_array_init(tmp, data, elem_nums, elem_size);
    if(res != ERR_CC_ARRAY_OK)
        goto fail3;

    *self = tmp;

    return ERR_CC_ARRAY_OK;


fail3:
    cc_free(data);
fail2:
    cc_free(tmp);
fail1:
    return res;
}

int cc_array_delete(struct cc_array *self, cc_delete_fn_t remove_fn)
{
    if (remove_fn) {
        for (cc_size_t i = 0; i < self->elem_nums; i++) {
            void *elem = self->data + i * self->elem_size;
            remove_fn(elem);
        }
    }
    cc_free((void *) (self->data));
    cc_free(self);
    return ERR_CC_ARRAY_OK;
}

static inline void cc_array_get_(struct cc_array *self, cc_size_t index, void *result)
{
    memcpy(result, self->data + index * self->elem_size, self->elem_size);
}

static inline void cc_array_set_(struct cc_array *self, cc_size_t index, void *value)
{
    memcpy(self->data + index * self->elem_size, value, self->elem_size);
}

inline int cc_array_is_vaild_index(struct cc_array *self, cc_size_t index)
{
    return index < self->elem_nums;
}

int cc_array_get_ref(struct cc_array *self, cc_size_t index, void **ref)
{
    if(!cc_array_is_vaild_index(self,index)) return ERR_CC_ARRAY_INVALID_ARG;

    *ref = self->data + index * self->elem_size;
    return ERR_CC_ARRAY_OK;
}

int cc_array_get_unsafe(struct cc_array *self, cc_size_t index, void *result)
{
    cc_array_get_(self,index,result);
    return ERR_CC_ARRAY_OK;
}

int cc_array_get(struct cc_array *self, cc_size_t index, void *result)
{
    if(!cc_array_is_vaild_index(self,index)) return ERR_CC_ARRAY_INVALID_ARG;
    if(result == NULL) return ERR_CC_ARRAY_INVALID_ARG;

    cc_array_get_(self,index,result);
    return ERR_CC_ARRAY_OK;
}

int cc_array_set_unsafe(struct cc_array *self, cc_size_t index, void *value)
{
    cc_array_set_(self,index,value);
    return ERR_CC_ARRAY_OK;
}

int cc_array_set(struct cc_array *self, cc_size_t index, void *value)
{
    if(!(cc_array_is_vaild_index(self,index))) return ERR_CC_ARRAY_INVALID_ARG;
    if(value == NULL) return ERR_CC_ARRAY_INVALID_ARG;

    cc_array_set_(self,index,value);
    return ERR_CC_ARRAY_OK;
}

int cc_array_cmp(struct cc_array *self, cc_cmp_fn_t cmp, cc_size_t i, cc_size_t j)
{
    return cmp(self->data + i * self->elem_size, self->data + j * self->elem_size);
}

int cc_array_swap(struct cc_array *self, cc_size_t i, cc_size_t j)
{
    if(!cc_array_is_vaild_index(self, i)) return ERR_CC_ARRAY_INVALID_ARG;
    if(!cc_array_is_vaild_index(self, j)) return ERR_CC_ARRAY_INVALID_ARG;

    unsigned char tmp[self->elem_size];
    cc_array_get_(self, i, tmp);
    memcpy(self->data + i * self->elem_size, self->data + j * self->elem_size, self->elem_size);
    cc_array_set_(self, j, tmp);
    return ERR_CC_ARRAY_OK;
}

int cc_array_reverse(struct cc_array *self, cc_size_t start, cc_size_t end)
{
    cc_size_t mid,i;
    if(!cc_array_is_vaild_index(self, start)) return ERR_CC_ARRAY_INVALID_ARG;
    if(!cc_array_is_vaild_index(self, end)) return ERR_CC_ARRAY_INVALID_ARG;

    if(start == end) return ERR_CC_ARRAY_INVALID_ARG;

    start = (start < end)? start : end;

    mid = (end - start) / 2;
    for(i = 0; i < mid; i++) {
        CC_ATOMIC_BEGIN();
        cc_array_swap(self, start + i, end - i);
        CC_ATOMIC_END();
    }

    return ERR_CC_ARRAY_OK;
}

int cc_array_copy_index(struct cc_array *array_a, struct cc_array * array_b, cc_size_t index_a, cc_size_t index_b)
{
    if(array_a->elem_size != array_b->elem_size) return ERR_CC_ARRAY_INVALID_ARG;
    if(!cc_array_is_vaild_index(array_a, index_a)) return ERR_CC_ARRAY_INVALID_ARG;
    if(!cc_array_is_vaild_index(array_b, index_b)) return ERR_CC_ARRAY_INVALID_ARG;

    unsigned char *tmp = cc_malloc(array_a->elem_size);
    cc_array_get_(array_a, index_a, tmp);
    cc_array_set_(array_b, index_b, tmp);
    cc_free(tmp);
    return ERR_CC_ARRAY_OK;
}

int cc_array_resize_by_copy(cc_array_t *self, cc_size_t new_elem_nums) {
    int res = ERR_CC_ARRAY_OK;
    cc_size_t copy_elements = (new_elem_nums < self->elem_nums) ? new_elem_nums : self->elem_nums;
    cc_size_t copy_len = copy_elements * self->elem_size;
    unsigned char *temp = cc_malloc(self->elem_size * new_elem_nums);

    if (temp == NULL) {
        res = ERR_CC_ARRAY_MEM_ERR;
        goto fail1;
    }

    memcpy(temp, self->data, copy_len);
    cc_free(self->data);
    self->data = temp;
    self->elem_nums = new_elem_nums; // 更新元素数量

fail1:
    return res;
}





/*
    iter


*/

cc_iter_i_t cc_array_iter_interface = {
    .next = (cc_iter_next_fn_t) cc_array_iterator_next,
};

int cc_array_iterator_init(cc_array_iter_t *self, cc_array_t *data)
{
    if(data == NULL || self == NULL) return ERR_CC_COMMON_INVALID_ARG;

    self->data = data;
    self->cursor = 0;
    self->iterator = &cc_array_iter_interface;

    return ERR_CC_COMMON_OK;
}


int cc_array_iterator_new(cc_array_iter_t **self, cc_array_t *data)
{
    if(data == NULL) return ERR_CC_COMMON_INVALID_ARG;

    cc_array_iter_t *temp = cc_malloc(sizeof(*temp));
    if(temp == NULL) return ERR_CC_COMMON_MEM_ERR;

    cc_array_iterator_init(temp, data);

    *self = temp;
    return ERR_CC_COMMON_OK;
}

int cc_array_iterator_delete(cc_array_iter_t *self)
{
    if(self == NULL) return ERR_CC_COMMON_INVALID_ARG;
    free(self);
    return ERR_CC_COMMON_OK;
}

int cc_array_iterator_next(cc_array_iter_t *self,void **item, cc_size_t *index)
{
    int res = ERR_CC_COMMON_OK;
    if(self == NULL) return ERR_CC_COMMON_INVALID_ARG;
    if(try_reset_double_p(item) != ERR_CC_COMMON_OK) return ERR_CC_COMMON_INVALID_ARG;

    res = cc_array_get_ref(self->data, *index, item);
    if(res != ERR_CC_COMMON_OK) return res;

    if(index != NULL) {
        *index = self->cursor;
    }
    self->cursor++;
    return ERR_CC_COMMON_OK;
}
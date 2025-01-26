#include "cc_array.h"
#include <stdlib.h>
#include <string.h>

int cc_array_init(struct cc_array *self, unsigned char *data, size_t elem_nums, size_t elem_size)
{
    self->data = data;
    self->elem_nums = elem_nums;
    self->elem_size = elem_size;
    return 0;
}


int cc_array_new(struct cc_array **self, size_t elem_nums, size_t elem_size)
{
    struct cc_array *tmp = NULL;
    unsigned char* data = NULL;

    tmp = malloc(sizeof(struct cc_array));
    if(tmp == NULL)
        goto fail1;

    data = malloc(elem_size * elem_nums);
    if(data == NULL)
        goto fail2;

    if(cc_array_init(tmp, data, elem_nums, elem_size))
        goto fail3;

    *self = tmp;

    return 0;


fail3:
    free(data);
fail2:
    free(tmp);
fail1:
    return 1;
}

int cc_array_delete(struct cc_array *self)
{
    free((void *) (self->data));
    free(self);
    // self->data = NULL;
    // self = NULL;
    return 0;
}

static inline void cc_array_get_(struct cc_array *self, size_t index, void *result)
{
    memcpy(result, self->data + index * self->elem_size, self->elem_size);
}

static inline void cc_array_set_(struct cc_array *self, size_t index, void *value)
{
    memcpy(self->data + index * self->elem_size, value, self->elem_size);
}

inline int cc_array_is_vaild_index(struct cc_array *self, size_t index)
{
    return index < self->elem_nums;
}

int cc_array_get_ref(struct cc_array *self, size_t index, void **ref)
{
    if(!cc_array_is_vaild_index(self,index)) return 1;

    *ref = self->data + index * self->elem_size;
    return 0;
}

int cc_array_get_unsafe(struct cc_array *self, size_t index, void *result)
{
    cc_array_get_(self,index,result);
    return 0;
}

int cc_array_get(struct cc_array *self, size_t index, void *result)
{
    if(!cc_array_is_vaild_index(self,index)) return 1;
    if(result == NULL) return 2;

    cc_array_get_(self,index,result);
    return 0;
}

int cc_array_set_unsafe(struct cc_array *self, size_t index, void *value)
{
    cc_array_set_(self,index,value);
    return 0;
}

int cc_array_set(struct cc_array *self, size_t index, void *value)
{
    if(!(cc_array_is_vaild_index(self,index))) return 1;
    if(value == NULL) return 2;

    cc_array_set_(self,index,value);
    return 0;
}

int cc_array_cmp(struct cc_array *self, cc_cmp_fn_t cmp, size_t i, size_t j)
{
    return cmp(self->data + i * self->elem_size, self->data + j * self->elem_size);
}

int cc_array_swap(struct cc_array *self, size_t i, size_t j)
{
    if(!cc_array_is_vaild_index(self, i)) return 1;
    if(!cc_array_is_vaild_index(self, j)) return 2;

    unsigned char tmp[self->elem_size];
    cc_array_get_(self, i, tmp);
    memcpy(self->data + i * self->elem_size, self->data + j * self->elem_size, self->elem_size);
    cc_array_set_(self, j, tmp);
    return 0;
}

int cc_array_reverse(struct cc_array *self, size_t start, size_t end)
{
    size_t mid,i;
    if(!cc_array_is_vaild_index(self, start)) return 1;
    if(!cc_array_is_vaild_index(self, end)) return 2;

    if(start == end) return 3;

    start = (start < end)? start : end;

    mid = (end - start) / 2;
    for(i = 0; i < mid; i++)
        cc_array_swap(self, start + i, end - i);

    return 0;
}



int cc_array_copy_index(struct cc_array *array_a, struct cc_array * array_b, size_t index_a, size_t index_b)
{
    if(array_a->elem_size != array_b->elem_size) return 1;
    if(!cc_array_is_vaild_index(array_a, index_a)) return 2;
    if(!cc_array_is_vaild_index(array_b, index_b)) return 3;

    unsigned char *tmp = malloc(array_a->elem_size);
    cc_array_get_(array_a, index_a, tmp);
    cc_array_set_(array_b, index_b, tmp);
    free(tmp);
    return 0;
}
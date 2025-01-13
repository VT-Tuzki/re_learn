#include "simple_array.h"
#include <stdlib.h>
#include <string.h>

int cc_array_init(struct s_array *self, unsigned char *data, size_t elem_nums, size_t elem_size)
{
    self->data = data;
    self->elem_nums = elem_nums;
    self->elem_size = elem_size;
    return 0;
}


int cc_array_new(struct s_array **self, size_t elem_nums, size_t elem_size)
{
    struct s_array *tmp = NULL;
    unsigned char* data = NULL;

    tmp = malloc(sizeof(struct s_array));
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

int cc_array_delete(struct s_array *self)
{
    free((void *) (self->data));
    free(self);
    self->data = NULL;
    self = NULL;
    return 0;
}

static inline void cc_array_get_(struct s_array *self, size_t index, void *result)
{
    memcpy(result, self->data + index * self->elem_size, self->elem_size);
}

static inline void cc_array_set_(struct s_array *self, size_t index, void *value)
{
    memcpy(self->data + index * self->elem_size, value, self->elem_size);
}

int cc_array_get_ref(struct s_array *self, size_t index, void **ref)
{
    if(index > self->elem_nums) return 1;

    *ref = self->data + index * (self->elem_size);
    return 0;
}

int cc_array_get_unsafe(struct s_array *self, size_t index, void *result)
{
    cc_array_get_(self,index,result);
    return 0;
}

int cc_array_get(struct s_array *self, size_t index, void *result)
{
    if(index > self->elem_nums) return 1;
    if(result == NULL) return 2;

    cc_array_get_(self,index,result);
    return 0;
}

int cc_array_set_unsafe(struct s_array *self, size_t index, void *value)
{
    cc_array_set_(self,index,value);
    return 0;
}

int cc_array_set(struct s_array *self, size_t index, void *value)
{
    if(index > self->elem_nums) return 1;
    if(value == NULL) return 2;

    cc_array_set_(self,index,value);
    return 0;
}
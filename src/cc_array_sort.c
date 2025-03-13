
#include "cc_stdint.h"

#include "cc_array.h"
#include "cc_array_sort.h"

static cc_size_t cc_array_sort_divide(struct cc_array *self,cc_cmp_fn_t cmp, cc_size_t start, cc_size_t end)
{
    cc_size_t left,right,mid;
    left = start;
    right = end;
    mid = start;
    while(left < right) {
        while (cc_array_cmp(self,cmp,right,mid) >= 0 && left < right)
            right--;
        while (cc_array_cmp(self,cmp,left,mid) <= 0 && left < right)
            left++;

        cc_array_swap(self, left, right);
    }
    cc_array_swap(self, left, mid);

    return left;
}

static void cc_array_sort_quick_(struct cc_array *self, cc_cmp_fn_t cmp, cc_size_t start, cc_size_t end)
{
    cc_size_t mid;
    if(start >= end)
        return;

    mid = cc_array_sort_divide(self, cmp, start, end);

    cc_array_sort_quick_(self, cmp, start, mid);
    cc_array_sort_quick_(self, cmp, mid + 1, end);

}

int cc_array_sort_quick(struct cc_array *self, cc_cmp_fn_t cmp)
{

    if(self == NULL) return 1;
    if(cmp == NULL) return 2;

    cc_array_sort_quick_(self,cmp,0,self->elem_nums - 1);

    return 0;
}

int cc_array_sort_bubble(struct cc_array *self, cc_cmp_fn_t cmp)
{
    cc_size_t i,j;
    if(self == NULL) return 1;
    if(cmp == NULL) return 2;

    for(i = 0; i < self->elem_nums - 1; i++)
        for(j = 0; j < self->elem_nums - 1 - i; j++)
            if(cc_array_cmp(self, cmp, j, j + 1) > 0)
                cc_array_swap(self, j, j + 1);

    return 0;
}

static int cc_array_sort_merge_array_merge(struct cc_array *self, cc_cmp_fn_t cmp, cc_size_t left, cc_size_t mid,cc_size_t right)
{
    cc_size_t i,j,k;

    struct cc_array *tmp_array;

    if(cc_array_new(&tmp_array, right - left + 1,self->elem_size, self->remove_fn)) {
        return 1;
    }

    i = left;
    j = mid + 1;
    k = 0;
    while(i <= mid && j <= right) {
        if(cc_array_cmp(self, cmp, i, j) < 0) {
            cc_array_copy_index(self,tmp_array,i++,k++);
        } else {
            cc_array_copy_index(self,tmp_array,j++,k++);
        }
    }
    while(i <= mid) {
        cc_array_copy_index(self,tmp_array,i++,k++);
    }
    while(j <= right) {
        cc_array_copy_index(self,tmp_array,j++,k++);
    }

    for (k = 0; k < right - left + 1; k++) {
        cc_array_copy_index(tmp_array,self,k,left + k);
    }

    cc_array_delete(tmp_array);
    return 0;
}

int cc_array_sort_merge_(struct cc_array *self, cc_cmp_fn_t cmp, cc_size_t start, cc_size_t end)
{
    cc_size_t mid;
    if(end == start) return 0;

    mid = start + ((end - start) >> 1);
    if(cc_array_sort_merge_(self, cmp, start, mid)) {
        return 1;
    }
    if(cc_array_sort_merge_(self, cmp, mid + 1, end)) {
        return 1;
    }

    return cc_array_sort_merge_array_merge(self, cmp, start, mid, end);
}


int cc_array_sort_merge(struct cc_array *self, cc_cmp_fn_t cmp)
{
    if(self == NULL) return 1;
    if(cmp == NULL) return 2;

    cc_array_sort_merge_(self,cmp, 0,self->elem_nums - 1);
    return 0;
}

#include "ds/array/cc_array.h"
#include "cc_array_sort.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

typedef int (*cc_cmp_fn_t)(void *left, void *right);
#define EPSILON 1e-6
int float_cmp(void *left,void *right)
{
    float fleft,fright;
    memcpy(&fleft,left,sizeof(float));
    memcpy(&fright,right,sizeof(float));

    if(fabs(fleft - fright) <= EPSILON) {
        return 0;
    }
    else {
        return ((int)(fleft - fright));
    }
}

#define TEST_LEN 100

int main(void) {
    struct cc_array array;
    unsigned char data[TEST_LEN * sizeof(float)];
    cc_size_t index;

    float i;
    float tmp;
    float *iter_tmp;



    assert(!cc_array_init(&array, data, TEST_LEN, sizeof(float), NULL));

    for (i = 0.27; i < TEST_LEN; i++)
        assert(!cc_array_set(&array, i, &i));

    for (i = 0.27; i < TEST_LEN; i++) {
        assert(!cc_array_get(&array, i, &tmp));
        printf("%f\n",tmp);
        assert(fabs(tmp - i) < 0.000001);
    }
    printf("------------------\n");
    assert(!cc_array_reverse(&array, 2, 4));
assert(!cc_array_reverse(&array, 20, 40));

assert(!cc_array_reverse(&array, 80, 99));
    for (i = 0.27; i < TEST_LEN; i++) {
        assert(!cc_array_get(&array, i, &tmp));
        printf("%f\n",tmp);
    }
    printf("------------------\n");
    assert(!cc_array_reverse(&array, 0, array.elem_nums -1));

    for (i = 0.27; i < TEST_LEN; i++) {
        assert(!cc_array_get(&array, i, &tmp));
        printf("%f\n",tmp);
    }

    printf("------sort----------\n");

    cc_array_sort_merge(&array, float_cmp);

    for (i = 0.27; i < TEST_LEN; i++) {
        assert(!cc_array_get(&array, i, &tmp));
        printf("%f\n",tmp);
    }

    // assert(!cc_array_iter_init(&iter, &array));
    // while (!cc_iter_next(&iter, &iter_tmp, &index))
    //     cc_debug_print("(index: %02d) %.9f\n", index, *iter_tmp);

    // cc_debug_print("\n");

    return 0;
}
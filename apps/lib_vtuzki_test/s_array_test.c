#include "ds/array/cc_array.h"
#include "ds/array/cc_array_sort.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

typedef int (*cc_cmp_fn_t)(void *left, void *right);

int float_cmp(void *left,void *right)
{
    return ((float*)left < (float*)right);
}



int main(void) {
    struct cc_array array;
    unsigned char data[10 * sizeof(float)];
    cc_size_t index;
    int res;

    float i;
    float tmp = 0;
    float *iter_tmp;

    printf("------init----------\n");

    res = cc_array_init(&array, data, 10, sizeof(float), NULL);
    if (res != 0) {
        printf("Error: cc_array_init failed with code %d\n", res);
        return 1;
    }

    for (i = 0.27; i < 10; i++) {
        res = cc_array_set(&array, i, &i);
        if (res != 0) {
            printf("Error: cc_array_set failed with code %d at i=%f\n", res, i);
            return 1;
        }
    }

    for (i = 0.27; i < 10; i++) {
        res = cc_array_get(&array, i, &tmp);
        if (res != 0) {
            printf("Error: cc_array_get failed with code %d at i=%f\n", res, i);
            return 1;
        }
        printf("%f\n",tmp);

        if (!(fabs(tmp - i) < 0.000001)) {
            printf("Error: value mismatch at i=%f, got %f\n", i, tmp);
            return 1;
        }
    }

    printf("------------------\n");
    res = cc_array_reverse(&array, 3, 7);
    if (res != 0) {
        printf("Error: cc_array_reverse failed with code %d\n", res);
        return 1;
    }

    for (i = 0.27; i < 10; i++) {
        res = cc_array_get(&array, i, &tmp);
        if (res != 0) {
            printf("Error: cc_array_get failed with code %d at i=%f\n", res, i);
            return 1;
        }
        printf("%f\n",tmp);
    }

    printf("------------------\n");
    res = cc_array_reverse(&array, 0, array.elem_nums -1);
    if (res != 0) {
        printf("Error: cc_array_reverse failed with code %d\n", res);
        return 1;
    }

    for (i = 0.27; i < 10; i++) {
        res = cc_array_get(&array, i, &tmp);
        if (res != 0) {
            printf("Error: cc_array_get failed with code %d at i=%f\n", res, i);
            return 1;
        }
        printf("%f\n",tmp);
    }

    printf("------sort----------\n");

    cc_array_sort_bubble(&array, float_cmp);

    for (i = 0.27; i < 10; i++) {
        res = cc_array_get(&array, i, &tmp);
        if (res != 0) {
            printf("Error: cc_array_get failed with code %d at i=%f\n", res, i);
            return 1;
        }
        printf("%f\n",tmp);
    }

    // assert(!cc_array_iter_init(&iter, &array));
    // while (!cc_iter_next(&iter, &iter_tmp, &index))
    //     cc_debug_print("(index: %02d) %.9f\n", index, *iter_tmp);

    // cc_debug_print("\n");
    //cc_array_delete(&array);
    return 0;
}
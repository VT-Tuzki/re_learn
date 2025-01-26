#include "cc_array.h"
#include "cc_array_sort.h"
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

    float i;
    float tmp;



    assert(!cc_array_init(&array, data, 10, sizeof(float)));

    for (i = 0.27; i < 10; i++)
        assert(!cc_array_set(&array, i, &i));

    for (i = 0.27; i < 10; i++) {
        assert(!cc_array_get(&array, i, &tmp));
        printf("%f\n",tmp);
        assert(fabs(tmp - i) < 0.000001);
    }
    printf("------------------\n");
    assert(!cc_array_reverse(&array, 3, 7));

    for (i = 0.27; i < 10; i++) {
            assert(!cc_array_get(&array, i, &tmp));
            printf("%f\n",tmp);
    }
printf("------------------\n");
    assert(!cc_array_reverse(&array, 0, array.elem_nums -1));

for (i = 0.27; i < 10; i++) {
        assert(!cc_array_get(&array, i, &tmp));
        printf("%f\n",tmp);
    }

    printf("------sort----------\n");

    cc_array_sort_bubble(&array, float_cmp);

    for (i = 0.27; i < 10; i++) {
        assert(!cc_array_get(&array, i, &tmp));
        printf("%f\n",tmp);
    }

    // assert(!cc_array_iter_init(&iter, &array));
    // while (!cc_iter_next(&iter, &iter_tmp, &index))
    //     cc_debug_print("(index: %02d) %.9f\n", index, *iter_tmp);

    // cc_debug_print("\n");
    //cc_array_delete(&array);
    return 0;
}
#include "simple_array.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    struct cc_array array;
    unsigned char data[10 * sizeof(float)];
    size_t index;

    float i;
    float tmp;
    float *iter_tmp;



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

    // assert(!cc_array_iter_init(&iter, &array));
    // while (!cc_iter_next(&iter, &iter_tmp, &index))
    //     cc_debug_print("(index: %02d) %.9f\n", index, *iter_tmp);

    // cc_debug_print("\n");

    return 0;
}
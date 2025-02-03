#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "cc_array.h"
#include "cc_stack.h"
#include "cc_array_stack.h"


#include <assert.h>
#include <string.h>
#include <math.h>

#define TEST_LEN 10
#define TEST_SIZE sizeof(float)


#define EPSILON 1e-6
int float_cmp(void *left,void *right)
{
    printf("left:%f right:%f\n",*(float *)left,*(float *)right);
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

float offset = 0.27;
float tmp = 0.27;
float *tmp_ref;
float tmp_1;
int main() {


    size_t elem_nums = 0;
    size_t empty_space = 0;
    int i = 0;
    struct cc_array_stack *stack;
    assert(!cc_array_stack_new(&stack, TEST_LEN, TEST_SIZE));

    empty_space = 0;
    assert(!cc_array_stack_space(stack, &empty_space));
    assert(empty_space == (size_t)(TEST_LEN));

    for(i = 0; i < TEST_LEN; i++) {

        tmp = i + offset;
        printf("i: %d tmp:%f\n",i,tmp);
        assert(!cc_stack_push(stack, &tmp));
        assert(!cc_array_stack_elem_nums(stack, &elem_nums));
        assert(elem_nums == (size_t)(i + 1));
        assert(!cc_array_stack_space(stack, &empty_space));
        assert(empty_space == (size_t)(TEST_LEN - i - 1));
        printf("i: %d top:%ld\n",i,stack->top);
    }

    assert(!cc_array_stack_space(stack, &empty_space));
    printf("%ld\n",empty_space);
    assert(empty_space == (size_t)(0));

    assert(cc_stack_push(stack, &tmp) == 1);
    assert(cc_array_stack_push(stack, &tmp) == 1);

    tmp = TEST_LEN - 1 + offset;
    tmp_ref = malloc(sizeof(*tmp_ref));
    assert(!cc_stack_peek(stack, (void **)tmp_ref));
    assert(float_cmp(&tmp,tmp_ref) == 0);
    printf("--%f\n",*tmp_ref);
    assert(!cc_array_stack_peek(stack, tmp_ref));
    printf("--%f\n",*tmp_ref);
    assert(float_cmp(&tmp,tmp_ref) == 0);

    assert(!cc_array_stack_peek(stack, tmp_ref));
    assert(float_cmp(&tmp,tmp_ref) == 0);

    for(i = 0; i < TEST_LEN; i++) {
        assert(!cc_stack_pop(stack, (void **)tmp_ref));
        tmp = TEST_LEN - i - 1 + offset;
        assert(float_cmp(&tmp,tmp_ref) == 0);

        assert(!cc_array_stack_elem_nums(stack, &elem_nums));
        assert(elem_nums == (size_t)(TEST_LEN - i - 1));
        assert(!cc_array_stack_space(stack, &empty_space));
        assert(empty_space == (size_t)(i + 1));
    }


    assert(!cc_array_stack_elem_nums(stack, &elem_nums));
    assert(elem_nums == (size_t)(0));


    assert(!cc_array_stack_space(stack, &empty_space));
    assert(empty_space == (size_t)(TEST_LEN));

    assert(cc_stack_pop(stack, (void **)tmp_ref) == 1);
    assert(cc_array_stack_pop(stack, tmp_ref) == 1);

    assert(!cc_array_stack_delete(stack));
    free(tmp_ref);
    return 0;
}
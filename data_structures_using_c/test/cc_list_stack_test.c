#include "cc_list_stack.h"
#include <assert.h>
#include <stdint.h>

#include "cc_mem.h"
#include <math.h>
#include "cc_dbg.h"

struct test_node {
    cc_size_t number;
    char   name[20];
};
typedef struct test_node test_node_t;

int print_list_node_data(test_node_t *data)
{
    if(data == NULL) {
        log_warn("temp -> NULL");
    }
    //printf("num: %d name: %s\n",data->number,data->name);
    return 0;
}

#define LOOP_LEN  3


int main() {

    cc_list_stack_t *list_stack;
    test_node_t *temp = NULL;
    assert(cc_list_stack_new(&list_stack, (cc_delete_fn_t)cc_free) == ERR_CC_COMMON_OK);

    assert(cc_stack_peek(list_stack, (void **)&temp) == ERR_CC_STACK_EMPTY);
    assert(cc_stack_pop(list_stack, (void **)&temp) == ERR_CC_STACK_EMPTY);


    temp = cc_malloc(sizeof(*temp));
    strcpy(temp->name,"list_stack");
    temp->number = 1;
    assert(cc_stack_push(list_stack, temp) == ERR_CC_COMMON_OK);

    temp = NULL;
    assert(cc_stack_peek(list_stack, (void **)&temp) == ERR_CC_COMMON_OK);
    print_list_node_data(temp);
    assert((temp->number == 1));
    assert(cc_stack_pop(list_stack, (void **)&temp) == ERR_CC_COMMON_OK);
    cc_free(temp);
    assert(cc_stack_pop(list_stack, (void **)&temp) == ERR_CC_STACK_EMPTY);

    for(int i = 0; i < LOOP_LEN; i++) {
        temp = cc_malloc(sizeof(*temp));
        check(temp,"temp cc_malloc failed");
        strcpy(temp->name,"list_stack");
        temp->number = i;
        assert(cc_stack_push(list_stack, temp) == ERR_CC_COMMON_OK);
    }
    temp = NULL;
    assert(cc_stack_peek(list_stack, (void **)&temp) == ERR_CC_COMMON_OK);
    assert(temp->number == LOOP_LEN - 1);

    temp = NULL;
    while(cc_stack_peek(list_stack, (void **)&temp) == ERR_CC_STACK_EMPTY)
    {
        assert(cc_stack_peek(list_stack, (void **)&temp) == ERR_CC_COMMON_OK);
        print_list_node_data(temp);
        cc_free(temp);
    }

    cc_list_stack_delete(list_stack);

    return 0;


error:
    exit(1);
}
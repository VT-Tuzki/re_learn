#ifndef __CC_STACK_H
#define __CC_STACK_H

#include "../../core/cc_common.h"
#include "../../core/cc_stdint.h"

typedef int (*cc_stack_push_fn_t)(void *self, void *data);
typedef int (*cc_stack_pop_fn_t)(void *self, void **result);
typedef int (*cc_stack_peek_fn_t)(void *self, void**result);


struct cc_stack_i {
    cc_stack_push_fn_t push;
    cc_stack_pop_fn_t pop;
    cc_stack_peek_fn_t peek;
};
typedef struct cc_stack_i cc_stack_i_t;

static inline int cc_stack_push(void *self, void *data) {
    return (*(struct cc_stack_i**)self)->push(self, data);
}

static inline int cc_stack_pop(void *self, void **result) {
    return (*(struct cc_stack_i**)self)->pop(self, result);
}

static inline int cc_stack_peek(void *self, void **result) {
    return (*(struct cc_stack_i**)self)->peek(self, result);
}



#endif
#ifndef __CC_ITER_H__
#define __CC_ITER_H__

#include "core/cc_stdint.h"
#include "core/cc_common.h"
typedef int (*cc_iter_next_fn_t) (void *self, void **item, cc_size_t *index);

struct cc_iter_i {
    cc_iter_next_fn_t next;
};
typedef struct cc_iter_i cc_iter_i_t;

typedef enum {
    ERR_CC_ITER_OK = ERR_CC_COMMON_OK,
    ERR_CC_ITER_END = 0xA1,
} cc_iter_err;


static inline int cc_iter_next(void *self, void *item, cc_size_t *index) {
    return (*(cc_iter_i_t **)self)->next(self, (void **)item, index);
}

#endif
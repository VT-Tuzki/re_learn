#ifndef __CC_ITER_H__
#define __CC_ITER_H__

#include "cc_stdint.h"

typedef int (*cc_iter_next_fn_t) (void *self, void **item, cc_size_t *index);

struct cc_iter_i {
    cc_iter_next_fn_t next;
};
typedef struct cc_iter_i cc_iter_i_t;



#endif
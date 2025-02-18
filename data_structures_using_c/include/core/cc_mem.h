#ifndef __CC_MEM_H_
#define __CC_MEM_H_

#include "core/cc_stdint.h" // 为了 size_t
#include "core/cc_common.h"
#ifdef __cplusplus
extern "C" {
#endif



/* 自定义内存分配器开关 */
#ifndef CC_USE_CUSTOM_ALLOCATOR
# include <stdlib.h>  // 包含标准库内存函数

static int adapter_free(void *obj) {
    free(obj);
    return ERR_CC_COMMON_OK;
}

# define cc_malloc  malloc
# define cc_free    adapter_free
# define cc_calloc  calloc
# define cc_realloc realloc

#else
/* 自定义内存函数声明 */
void* cc_malloc(size_t size);
void  cc_free(void* ptr);
void* cc_calloc(size_t num, size_t size);
void* cc_realloc(void* ptr, size_t new_size);

#endif  // CC_USE_CUSTOM_ALLOCATOR





#ifdef __cplusplus
}
#endif

#endif  // __CC_MEM_H_
#ifndef __CC_MEM_H_
#define __CC_MEM_H_

#include "cc_stdint.h" // 为了 size_t
#include "cc_common.h"
# include <stdlib.h>  // 包含标准库内存函数

#ifdef __cplusplus
extern "C" {
#endif



/* 自定义内存分配器开关 */
#ifdef CC_USE_CUSTOM_ALLOCATOR

/* 自定义内存函数声明 */
void* malloc(size_t size);
void  free(void* ptr);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t new_size);

#endif  // CC_USE_CUSTOM_ALLOCATOR


static int adapter_free(void *obj) {
    free(obj);
    return ERR_CC_COMMON_OK;
}


#ifdef __cplusplus
}
#endif

#endif  // __CC_MEM_H_
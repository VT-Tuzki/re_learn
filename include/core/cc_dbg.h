#ifndef __CC_DBG_H__
#define __CC_DBG_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG [%s]%s:%d: " M "\n", __func__, __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] ([%s]%s:%d: errno: %s) " M "\n", __func__, __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] ([%s]%s:%d: errno: %s) " M "\n",  __func__,__FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] ([%s]%s:%d) " M "\n",  __func__,__FILE__, __LINE__, ##__VA_ARGS__)

#define log_res_err(A,M, ...) fprintf(stderr, "[ERROR] ([%s]%s:%d: errno: %s res:%d) " M "\n", __func__, __FILE__, __LINE__, clean_errno(), (A), ##__VA_ARGS__)


#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_res_ok(A, M, ...) if(((A) != 0)) { log_res_err((A), M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }





#endif

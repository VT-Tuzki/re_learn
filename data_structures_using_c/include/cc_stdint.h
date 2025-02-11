#ifndef __CC_STDINT_H__
#define __CC_STDINT_H__

/*-----------------------------------------------
 * 自定义基础类型定义
 * 根据平台特性手动适配
 *---------------------------------------------*/

/* 检查编译器是否支持 stdint.h */
#ifdef AAAA
  #include <stdint.h>
  #include <stddef.h>  // 包含 cc_size_t 和 ptrdiff_t

  /* 直接复用标准库类型 */
  typedef uint8_t   cc_uint8_t;
  typedef int8_t    cc_int8_t;
  typedef uint16_t  cc_uint16_t;
  typedef int16_t   cc_int16_t;
  typedef uint32_t  cc_uint32_t;
  typedef int32_t   cc_int32_t;
  typedef size_t    cc_size_t;
  typedef ptrdiff_t cc_ptrdiff_t;

#else
  /*-----------------------------------------------
   * 手动定义类型（假设典型32位嵌入式平台）
   * 需根据实际编译器调整
   *---------------------------------------------*/
  typedef unsigned char      cc_uint8_t;
  typedef signed char        cc_int8_t;
  typedef unsigned short     cc_uint16_t;
  typedef signed short       cc_int16_t;
  typedef unsigned int       cc_uint32_t;
  typedef signed int         cc_int32_t;

  /* 定义 cc_size_t 和 ptrdiff_t */
  typedef unsigned int       cc_size_t;     // 假设平台用 32-bit 表示大小
  typedef int                cc_ptrdiff_t;  // 指针差值类型
#endif

/*-----------------------------------------------
 * 可选：定义布尔类型
 *---------------------------------------------*/
#ifndef __CC_BOOL_DEFINED__
#define __CC_BOOL_DEFINED__
typedef enum {
    cc_false = 0,
    cc_true  = 1
} cc_bool_t;
#endif

/*-----------------------------------------------
 * 可选：定义NULL
 *---------------------------------------------*/
#ifndef __CC_NULL_DEFINED__
#define __CC_NULL_DEFINED__
// 如果标准库未定义 NULL，我们手动定义
#ifndef NULL
#define NULL ((void *)0)
#endif
#endif // __CC_NULL_DEFINED__

/*-----------------------------------------------
 * 类型安全检查（可选）
 * 编译时静态断言确保类型大小符合预期
 *---------------------------------------------*/
#ifdef __GNUC__
  #define CC_STATIC_ASSERT(expr) _Static_assert(expr, #expr)
#else
  #define CC_STATIC_ASSERT(expr) typedef char __cc_static_assert[(expr) ? 1 : -1]
#endif

/* 检查类型大小 */
CC_STATIC_ASSERT(sizeof(cc_uint8_t)  == 1);
CC_STATIC_ASSERT(sizeof(cc_uint16_t) == 2);
CC_STATIC_ASSERT(sizeof(cc_uint32_t) == 4);

#endif // __CC_STDINT_H__
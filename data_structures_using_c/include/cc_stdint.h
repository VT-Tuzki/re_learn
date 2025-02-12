#ifndef __CC_STDINT_H__
#define __CC_STDINT_H__
/*------------------------------
  静态断言实现 (跨平台支持)
------------------------------*/
#ifdef __cplusplus
  /* C++环境直接使用static_assert */
  #define CC_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#else
  #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    /* C11及以上环境使用标准_Static_assert */
    #define CC_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
  #elif defined(__GNUC__)
    /* 非C11标准的GCC/Clang编译器 */
    #define CC_STATIC_ASSERT(cond, msg) typedef char __cc_assert_##__LINE__[(cond)?1:-1] __attribute__((unused))
  #else
    /* 通用回退方案 */
    #define CC_STATIC_ASSERT(cond, msg) typedef char __cc_assert_##__LINE__[(cond)?1:-1]
  #endif
#endif  // __cplusplus


/*------------------------------
  C99标准兼容模式（优先使用标准定义）
------------------------------*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

  #include <stdint.h>

  // 映射到标准类型
  typedef uint8_t   cc_uint8_t;
  typedef int8_t    cc_int8_t;
  typedef uint16_t  cc_uint16_t;
  typedef int16_t   cc_int16_t;
  typedef uint32_t  cc_uint32_t;
  typedef int32_t   cc_int32_t;
  typedef uint64_t  cc_uint64_t;
  typedef int64_t   cc_int64_t;
#else

/*------------------------------
  C89环境定制实现
------------------------------*/
  typedef unsigned char   cc_uint8_t;   // 精准8位
  typedef signed char     cc_int8_t;

/*
 * 强制保证16位
 * 根据架构调整类型载体
 */
  #if defined(__INT_MAX__) && (__INT_MAX__ == 32767)
    typedef unsigned int      cc_uint16_t;  // 16位系统
    typedef signed int        cc_int16_t;
  #else
    typedef unsigned short    cc_uint16_t;  // 通用环境
    typedef signed short      cc_int16_t;
    CC_STATIC_ASSERT(sizeof(short) == 2, "需保证16位类型有效");
  #endif

/*
 * 强制保证32位
 * 通过编译器适配提升准确性
 */
  #if defined(__GNUC__)
    typedef __UINT32_TYPE__   cc_uint32_t;  // 使用GCC内建类型
    typedef __INT32_TYPE__    cc_int32_t;
  #elif defined(_MSC_VER)
    typedef unsigned __int32  cc_uint32_t;  // MSVC专用
    typedef __int32           cc_int32_t;
  #else
    typedef unsigned long     cc_uint32_t;  // 通用回退方案
    typedef signed long       cc_int32_t;
    CC_STATIC_ASSERT(sizeof(long) >= 4, "32位类型验证失败");
  #endif

/*
 * 64位类型双模式支持
 */
  #if (__SIZEOF_LONG_LONG__ == 8)
    typedef unsigned long long  cc_uint64_t;  // 主流编译器支持
    typedef signed long long    cc_int64_t;
  #elif defined(_MSC_VER)
    typedef unsigned __int64    cc_uint64_t;  // MSVC专用
    typedef __int64             cc_int64_t;
  #else
    #pragma message("警告：当前平台无法确保64位类型准确性，部分功能受限")
    struct cc_uint64 { cc_uint32_t lo, hi; };  // Fallback手动实现
    struct cc_int64 { cc_int32_t lo, hi; };
    typedef struct cc_uint64    cc_uint64_t;
    typedef struct cc_int64     cc_int64_t;
#endif

#endif

/*------------------------------
  严格类型尺寸验证（嵌入式开发关键保障）
------------------------------*/
CC_STATIC_ASSERT(sizeof(cc_uint8_t)  == 1,  "8位无符号异常");
CC_STATIC_ASSERT(sizeof(cc_int8_t)   == 1,  "8位有符号异常");
CC_STATIC_ASSERT(sizeof(cc_uint16_t) == 2,  "16位无符号异常");
CC_STATIC_ASSERT(sizeof(cc_int16_t)  == 2,  "16位有符号异常");
CC_STATIC_ASSERT(sizeof(cc_uint32_t) == 4,  "32位无符号异常");
CC_STATIC_ASSERT(sizeof(cc_int32_t)  == 4,  "32位有符号异常");
#if !defined(__STDC_NO_LONG_LONG__)
CC_STATIC_ASSERT(sizeof(cc_uint64_t) == 8,  "64位无符号异常");
CC_STATIC_ASSERT(sizeof(cc_int64_t) == 8, "64位有符号异常");
#endif

/*------------------------------
  地址宽度基准类型
------------------------------*/
// 强制要求cc_size_t与指针尺寸完全一致 [核心修改]
#if defined(__SIZE_TYPE__)  /* GCC/Clang原生支持 */
  typedef __SIZE_TYPE__     cc_size_t;   // 无缝对接编译器实现
#elif defined(_MSC_VER)
  typedef size_t            cc_size_t;   // MSVC环境
#else                       /* 非标准编译环境 */
  /* 手动选择指针宽度 */
  #if defined(__POINTER_WIDTH_64__) || defined(_WIN64)
    typedef unsigned long long  cc_size_t;  // 64位环境
  #elif defined(__16BIT__) || (__SIZEOF_POINTER__ == 2)
    typedef unsigned short      cc_size_t;  // 16位环境
  #elif defined(__8BIT__) || (__SIZEOF_POINTER__ == 1)
    typedef unsigned char       cc_size_t;  // 8位环境
  #else
    typedef unsigned int        cc_size_t;  // 默认32位
  #endif
#endif

CC_STATIC_ASSERT(
  sizeof(cc_size_t) == sizeof(void*),
  "cc_size_t必须与指针尺寸严格一致"
);

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


/*------------------------------
  printf格式化宏
------------------------------*/
// // size_t格式化动态适配
// #if (sizeof(cc_size_t) == 4)
//   #define CC_PRI_SIZET    "lu"
//   #define CC_SCN_SIZET    "lu"
// #elif (sizeof(cc_size_t) == 2)
//   #define CC_PRI_SIZET    "u"
//   #define CC_SCN_SIZET    "hu"
// #elif (sizeof(cc_size_t) == 1)
//   #define CC_PRI_SIZET    "hhu"
//   #define CC_SCN_SIZET    "hhu"
// #endif

#endif // __CC_STDINT_H__
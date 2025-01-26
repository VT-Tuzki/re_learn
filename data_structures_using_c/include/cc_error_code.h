#ifndef __CC_ERROR_CODE_H
#define __CC_ERROR_CODE_H
#include <stddef.h>
#include <stdlib.h>
/*
    ERROR CODE

    such as     0xA002B1
                A0-02-B-1
                A0-CC_ARRAY
                02-


*/

enum error_element {
    CC_ARRAY = 0xA0,
    CC_ARRAY_SORT = 0xA1,
    CC_ARRAY_STACK = 0xA2,
};

// #define MERGE_BODY(a, b)    a ## b
// #define MERGE(a,b)     MERGE_BODY(a, b)
// #define UNIQUENAME(name) merge(name, __COUNTER__)


// #define NO_ERROR_CODE ((unsigned int)0x00000000)
// #define ERROR_CODE_(AA,BB,CC,DD) \
//     {                            \
//         __typeof__(AA)  = (A);


//     }

// #define __NSX_PASTE__(A,B) A##B

// #define MIN(A,B) __NSMIN_IMPL__(A,B,__COUNTER__)

// #define __NSMIN_IMPL__(A,B,L) ({ __typeof__(A) __NSX_PASTE__(__a,L) = (A); \
//                                  __typeof__(B) __NSX_PASTE__(__b,L) = (B); \
//                                  (__NSX_PASTE__(__a,L) < __NSX_PASTE__(__b,L)) ? __NSX_PASTE__(__a,L) : __NSX_PASTE__(__b,L); \
//                               })

// #define setRegBit(reg, bit)           (reg |= (word)(reg##_##bit##_##MASK))
// #define err

#endif
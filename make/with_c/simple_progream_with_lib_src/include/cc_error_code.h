#ifndef __CC_ERROR_CODE_H
#define __CC_ERROR_CODE_H
#include <stddef.h>
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
}


#endif
#ifndef __CC_ATOMIC_H__
#define __CC_ATOMIC_H__


// 平台特定实现
#if defined(USE_ARM_CORTEX)
    #include "cmsis_compiler.h"
    #define CC_ATOMIC_BEGIN()   __disable_irq()
    #define CC_ATOMIC_END()     __enable_irq()
#elif defined(USE_RISCV)
    // RISC-V 实现样例
    #define CC_ATOMIC_BEGIN()   __asm__ volatile ("csrci mstatus, 0x8")
    #define CC_ATOMIC_END()     __asm__ volatile ("csrsi mstatus, 0x8")
#else
    #define CC_ATOMIC_BEGIN()   {}
    #define CC_ATOMIC_END()     {}
#endif


#endif // __CC_ATOMIC_H__
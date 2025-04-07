#ifndef __RISC_V_INTEGER_REG_H__
#define __RISC_V_INTEGER_REG_H__

#include "core/cc_stdint.h"



void rv32i_reset_registers();

uint32_t rv32i_read_reg(int reg_num, uint32_t *value);

uint32_t rv32i_write_reg(int reg_num, uint32_t value);


uint32_t rv32i_read_pc(uint32_t *value);
uint32_t rv32i_set_pc(uint32_t set_pc);


//Debug
void rv32i_print_reg();

/*
    Name        ABI Mnemonic    Meaning                 Preserved across calls?
    x0          zero            Zero                        — (Immutable)
    x1          ra              Return address              No
    x2          sp              Stack pointer               Yes
    x3          gp              Global pointer              — (Unallocatable)
    x4          tp              Thread pointer              — (Unallocatable)
    x5 - x7     t0 - t2         Temporary registers         No
    x8 - x9     s0 - s1         Callee-saved registers      Yes
    x10 - x17   a0 - a7         Argument registers          No
    x18 - x27   s2 - s11        Callee-saved registers      Yes
    x28 - x31   t3 - t6         Temporary registers         No


    关于x8:   如果选用帧指针(frame pointer) 则需要放置在x8 且调用时保留
    注: 标准ABI中 不应该在过程中修改 gp 和 tp
*/


#endif //__RISC_V_INTEGER_REG_H__
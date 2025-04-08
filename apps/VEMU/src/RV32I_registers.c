#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "RV32I_registers.h"

const char *regs[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
  };

rv32i_register_t rv32i_regs;


void rv32i_reset_registers() {
    for(int i = 0; i < 32; i++) {
        rv32i_regs.x[i] = 0;
    }
    rv32i_regs.pc = 0;
    return;
}

uint32_t rv32i_read_reg(int reg_num, uint32_t *value) {

    if( reg_num < 0 || reg_num >= 32 || value == NULL) {
        return 1;
    }

    *value = (reg_num == 0) ? 0 : rv32i_regs.x[reg_num];
    return 0;
}

uint32_t rv32i_write_reg(int reg_num, uint32_t value) {

    if( reg_num <= 0 || reg_num >= 32) {
        return 1;
    }

    rv32i_regs.x[reg_num] = value;

    return 0;
}


uint32_t rv32i_read_pc(uint32_t *value) {

    if(value == NULL) return 1;

    *value = rv32i_regs.pc;

    return 0;
}
uint32_t rv32i_set_pc(uint32_t set_pc) {
    rv32i_regs.pc = set_pc;
    return 0;
}


void rv32i_print_reg() {

    printf("-----reg------\n");
    printf("%-8s%-8x\n","pc",rv32i_regs.pc);
    for(int i = 0; i < 32; i++) {
      printf("%-8s%-8x\n", regs[i], rv32i_regs.x[i]);
    }

    printf("------end------\n");

    return;
}

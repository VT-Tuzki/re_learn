#ifndef __RV32I_INSTRUCTION_H__
#define __RV32I_INSTRUCTION_H__

#include "core/cc_stdint.h"

typedef union {
    struct {
        uint32_t funct7: 7;
        uint32_t rs2: 5;
        uint32_t rs1: 5;
        uint32_t funct3: 3;
        uint32_t rd : 5;
        uint32_t opcode : 7;
    } rv32i_rtype;

    struct {
        uint32_t imm11_0: 12;
        uint32_t rs1: 5;
        uint32_t funct3: 3;
        uint32_t rd : 5;
        uint32_t opcode : 7;
    } rv32i_itype;

    struct {
        uint32_t imm11_5: 7;
        uint32_t rs2: 5;
        uint32_t rs1: 5;
        uint32_t funct3: 3;
        uint32_t imm4_0 : 5;
        uint32_t opcode : 7;
    } rv32i_stype;

    struct {
        uint32_t imm12: 1;
        uint32_t imm10_5: 6;

        uint32_t rs2: 5;
        uint32_t rs1: 5;

        uint32_t funct3: 3;

        uint32_t imm4_1 : 4;
        uint32_t imm11 : 1;

        uint32_t opcode : 7;
    } rv32i_btype;

    struct {
        uint32_t imm31_12: 20;
        uint32_t rd : 5;
        uint32_t opcode : 7;
    } rv32i_utype;

    struct {
        uint32_t imm20: 1;
        uint32_t imm10_1: 9;
        uint32_t imm11 : 1;
        uint32_t imm19_12: 8;

        uint32_t rd : 5;
        uint32_t opcode : 7;
    } rv32i_jtype;

} rv32i_inst_t;

enum

uint32_t rv32i_opcode_fetch(rv32i_inst_t *inst);
uint32_t rv32i_opcode_decode(rv32i_inst_t inst);
uint32_t rv32i_opcode_execute(rv32i_inst_t inst);


uint32_t rv32i_exec_once();



#endif  //__RV32I_INSTRUCTION_H__

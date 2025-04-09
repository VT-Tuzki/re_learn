#include <stdio.h>
#include <string.h>

#include "RV32I_instruction.h"
#include "RV32I_memory.h"
#include "RV32I_registers.h"

#include "core/cc_stdint.h"
#include "ds/array/cc_array.h"
#include "core/cc_dbg.h"

#define RV32I_MAX_INSTRUCTIONS 105

typedef struct {
    rv32i_base_inst_t instructions[RV32I_MAX_INSTRUCTIONS];
    uint32_t count;
} rv32i_base_table_t;

rv32i_base_table_t rv32i_base_table = {};


uint32_t rv32i_exec_add (rv32i_decoded_inst_t decoded_inst) {

    uint32_t rs1_val, rs2_val;

    rv32i_read_reg(decoded_inst.rs1, &rs1_val);
    rv32i_read_reg(decoded_inst.rs2, &rs2_val);

    uint32_t result = rs1_val + rs2_val;

    rv32i_write_reg(decoded_inst.rd, result);

    printf("run rv32i add inst: %x \n", *(uint32_t*)&decoded_inst);
    printf("target reg:%d,value:%d rs1 reg:%d,value:%d,rs2 reg:%d,value:%d\n",decoded_inst.rd, result, decoded_inst.rs1, rs1_val, decoded_inst.rs2, rs2_val);

    return 0;
}

uint32_t rv32i_exec_sub (rv32i_decoded_inst_t decoded_inst) {
    uint32_t rs1_val, rs2_val;

    rv32i_read_reg(decoded_inst.rs1, &rs1_val);
    rv32i_read_reg(decoded_inst.rs2, &rs2_val);

    uint32_t result = rs1_val - rs2_val;

    rv32i_write_reg(decoded_inst.rd, result);

    printf("run rv32i sub inst %x\n", *(uint32_t*)&decoded_inst);
    printf("target reg:%d,value:%d rs1 reg:%d,value:%d,rs2 reg:%d,value:%d\n",decoded_inst.rd, result, decoded_inst.rs1, rs1_val, decoded_inst.rs2, rs2_val);

    return 0;
}

uint32_t rv32i_exec_addi (rv32i_decoded_inst_t decoded_inst) {
    uint32_t rs1_val;
    rv32i_read_reg(decoded_inst.rs1, &rs1_val);
    uint32_t result = rs1_val + decoded_inst.imm;
    rv32i_write_reg(decoded_inst.rd, result);

    printf("run rv32i addi inst: %x rs1:%u imm:%u rd:%u \n", *(uint32_t*)&decoded_inst, rs1_val, decoded_inst.imm, result);
    printf("target reg:%d,value:%d rs1 reg:%d,value:%d, imm:%d\n",decoded_inst.rd, result, decoded_inst.rs1, rs1_val, decoded_inst.imm);
    return 0;
}

uint32_t rv32i_inst_table_find(rv32i_inst_t inst, rv32i_decoded_inst_t *decoded_inst, rv32i_base_inst_t *base_inst) {
    uint32_t now_inst = *(uint32_t*) &inst;

    memset(decoded_inst, 0, sizeof(rv32i_decoded_inst_t));
    decoded_inst->inst = now_inst;
    uint8_t opcode = now_inst & 0x007F;

    if((opcode & 0x3) != 0x3) {
        return 1;
    }

    uint8_t opcode_6_2 = (opcode >> 2) & 0x001F;
    debug("inst: %04x, opcode_6_2: %u\n",*(uint32_t*) &inst, opcode_6_2);

    debug("rv32i_base_table.count:%d",rv32i_base_table.count);
    for (uint32_t i = 0; i < rv32i_base_table.count; i++) {
        rv32i_base_inst_t *curr_inst = &rv32i_base_table.instructions[i];
        if (curr_inst->opcode != opcode_6_2) {
            debug("curr_inst->opcode:%d   opcode_6_2:%d", curr_inst->opcode, opcode_6_2);
            continue;
        }
        debug("find: i:%d",i);

        switch(curr_inst->type) {
            case RV32I_INST_TYPE_R:
                decoded_inst->funct3 = inst.rv32i_rtype.funct3;
                decoded_inst->funct7 = inst.rv32i_rtype.funct7;
                decoded_inst->rs1 = inst.rv32i_rtype.rs1;
                decoded_inst->rs2 = inst.rv32i_rtype.rs2;
                decoded_inst->rd = inst.rv32i_rtype.rd;
                debug("rd:%d funct3:%d, rs1:%d",decoded_inst->rd, decoded_inst->funct3, decoded_inst->rs1);

                if (curr_inst->funct3 != decoded_inst->funct3 ||
                    curr_inst->funct7 != decoded_inst->funct7) {
                    continue;
                }
                break;

            case RV32I_INST_TYPE_I:
                decoded_inst->rd = inst.rv32i_itype.rd;
                decoded_inst->funct3 = inst.rv32i_itype.funct3;
                decoded_inst->rs1 = inst.rv32i_itype.rs1;
                debug("rd:%d funct3:%d, rs1:%d",decoded_inst->rd, decoded_inst->funct3, decoded_inst->rs1);
                if (curr_inst->funct3 != decoded_inst->funct3) {
                    debug("exit 1 \n");

                    continue;
                }
                //slli srli srai
                if (curr_inst->funct3 == 0b001 || curr_inst->funct3 == 0b101) {
                    decoded_inst->funct7 = (now_inst >> 25) & 0x7F;
                    if(curr_inst->funct7 != decoded_inst->funct7) {
                        debug("exit 2 \n");

                        continue;
                    }
                }
                break;

            case RV32I_INST_TYPE_S:
            case RV32I_INST_TYPE_B:
                decoded_inst->funct3 = inst.rv32i_stype.funct3;
                decoded_inst->rs1 = inst.rv32i_stype.rs1;
                decoded_inst->rs2 = inst.rv32i_stype.rs2;

                if (curr_inst->funct3 != decoded_inst->funct3) {
                    continue;
                }
                break;

            case RV32I_INST_TYPE_U:
            case RV32I_INST_TYPE_J:
                decoded_inst->rd = inst.rv32i_itype.rd;
                break;
            default:
                continue;
        }

        *base_inst = *curr_inst;
        decoded_inst->type = curr_inst->type;

        if(curr_inst->rv32i_inst_imm_fn != NULL) {
            uint32_t imm_res = curr_inst->rv32i_inst_imm_fn(inst, (uint32_t *)&decoded_inst->imm);
            if(imm_res != 0) return imm_res;
        }
        return 0;
    }
    return 3;
}




void rv32i_init_inst_table(void) {

    RV32I_INST_DECLARE(rv32i_base_table, "add", RV32I_INST_TYPE_R, RV32I_OPCODE_CAL, 0b000, 0b0000000, NULL, rv32i_exec_add);
    RV32I_INST_DECLARE(rv32i_base_table, "sub", RV32I_INST_TYPE_R, RV32I_OPCODE_CAL, 0b000, 0b0100000, NULL, rv32i_exec_sub);
    RV32I_INST_DECLARE(rv32i_base_table, "addi", RV32I_INST_TYPE_I, RV32I_OPCODE_CAL_I, 0b000, 0, rv32i_inst_get_i_imm, rv32i_exec_addi);

    return;
}
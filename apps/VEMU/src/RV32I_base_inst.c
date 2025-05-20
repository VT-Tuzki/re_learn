#include <stdio.h>
#include <string.h>

#include "RV32I_instruction.h"
#include "RV32I_memory.h"
#include "RV32I_registers.h"

#include "core/cc_stdint.h"
#include "ds/array/cc_array.h"
#include "core/cc_dbg.h"

#define RV32I_MAX_INSTRUCTIONS 105
#define BITMASK(bits) ((1ull << (bits)) - 1)
#define BITS(x, hi, lo) (((x) >> (lo)) & BITMASK((hi) - (lo) + 1)) // similar to x[hi:lo] in verilog
#define SEXT(x, len) ({ struct { int64_t n : len; } __x = { .n = x }; (uint64_t)__x.n; })


#define DEFINE_RV32I_R_TYPE_EXEC(name, operation) \
uint32_t rv32i_exec_##name (rv32i_decoded_inst_t decoded_inst) { \
    uint32_t rs1_val, rs2_val; \
    rv32i_read_reg(decoded_inst.rs1, &rs1_val); \
    rv32i_read_reg(decoded_inst.rs2, &rs2_val); \
    uint32_t result = operation; \
    rv32i_write_reg(decoded_inst.rd, result); \
    printf("run rv32i " #name " inst %x\n", *(uint32_t*)&decoded_inst); \
    printf("target reg:%d,value:%d rs1 reg:%d,value:%d,rs2 reg:%d,value:%d\n", \
           decoded_inst.rd, result, decoded_inst.rs1, rs1_val, decoded_inst.rs2, rs2_val); \
    return 0; \
}

#define DEFINE_RV32I_I_TYPE_EXEC(name, operation) \
uint32_t rv32i_exec_##name (rv32i_decoded_inst_t decoded_inst) { \
    uint32_t rs1_val; \
    rv32i_read_reg(decoded_inst.rs1, &rs1_val); \
    uint32_t result = operation; \
    rv32i_write_reg(decoded_inst.rd, result); \
    printf("run rv32i " #name " inst: %x rs1:%u imm:%u rd:%u \n", \
           *(uint32_t*)&decoded_inst, rs1_val, decoded_inst.imm, result); \
    printf("target reg:%d,value:%d rs1 reg:%d,value:%d, imm:%d\n", \
           decoded_inst.rd, result, decoded_inst.rs1, rs1_val, decoded_inst.imm); \
    return 0; \
}

typedef struct {
    rv32i_base_inst_t instructions[RV32I_MAX_INSTRUCTIONS];
    uint32_t count;
} rv32i_base_table_t;

rv32i_base_table_t rv32i_base_table = {};

DEFINE_RV32I_R_TYPE_EXEC(add, rs1_val + rs2_val)
DEFINE_RV32I_R_TYPE_EXEC(sub, rs1_val - rs2_val)
DEFINE_RV32I_R_TYPE_EXEC(sll, rs1_val << rs2_val)
DEFINE_RV32I_R_TYPE_EXEC(slt, ((int32_t)rs1_val < (int32_t)rs2_val) ? 1 : 0)
DEFINE_RV32I_R_TYPE_EXEC(sltu, ((uint32_t)rs1_val < (uint32_t)rs2_val) ? 1 : 0)
DEFINE_RV32I_R_TYPE_EXEC(xor, rs1_val ^ rs2_val)
DEFINE_RV32I_R_TYPE_EXEC(srl, rs1_val >> rs2_val)
DEFINE_RV32I_R_TYPE_EXEC(sra, (int32_t)rs1_val >> rs2_val)
DEFINE_RV32I_R_TYPE_EXEC(or, rs1_val | rs2_val)
DEFINE_RV32I_R_TYPE_EXEC(and, rs1_val & rs2_val)

DEFINE_RV32I_I_TYPE_EXEC(addi, rs1_val + decoded_inst.imm)
DEFINE_RV32I_I_TYPE_EXEC(slti, ((int32_t)rs1_val < (int32_t)decoded_inst.imm) ? 1 : 0)
DEFINE_RV32I_I_TYPE_EXEC(sltiu, ((uint32_t)rs1_val < (uint32_t)decoded_inst.imm) ? 1 : 0)
DEFINE_RV32I_I_TYPE_EXEC(xori, rs1_val ^ decoded_inst.imm)
DEFINE_RV32I_I_TYPE_EXEC(ori, rs1_val | decoded_inst.imm)
DEFINE_RV32I_I_TYPE_EXEC(andi, rs1_val & decoded_inst.imm)
DEFINE_RV32I_I_TYPE_EXEC(slli, rs1_val << (decoded_inst.imm & 0x1F))
DEFINE_RV32I_I_TYPE_EXEC(srli, rs1_val >> BITS(decoded_inst.imm, 4, 0))
DEFINE_RV32I_I_TYPE_EXEC(srai, ((int32_t)rs1_val) >> (decoded_inst.imm & 0x1F))

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

    //TODO : 将 fact3 fact7 改为字符串形式判断

    RV32I_INST_DECLARE(rv32i_base_table, "add",     RV32I_INST_TYPE_R, RV32I_OPCODE_CAL,    0b000, 0b0000000,   NULL,                   rv32i_exec_add);
    RV32I_INST_DECLARE(rv32i_base_table, "sub",     RV32I_INST_TYPE_R, RV32I_OPCODE_CAL,    0b000, 0b0100000,   NULL,                   rv32i_exec_sub);
    // RV32I_INST_DECLARE(rv32i_base_table, "sll",     RV32I_INST_TYPE_R, RV32I_OPCODE_CAL,    0b001, 0b0000000,   NULL,                   rv32i_exec_sll);
    // RV32I_INST_DECLARE(rv32i_base_table, "slt",     RV32I_INST_TYPE_R, RV32I_OPCODE_CAL,    0b010, 0b0000000,   NULL,                   rv32i_exec_slt);
    // RV32I_INST_DECLARE(rv32i_base_table, "sltu",    RV32I_INST_TYPE_R, RV32I_OPCODE_CAL,    0b011, 0b0000000,   NULL,                   rv32i_exec_sltu);
    // RV32I_INST_DECLARE(rv32i_base_table, "xor",     RV32I_INST_TYPE_R, RV32I_OPCODE_CAL,    0b100, 0b0000000,   NULL,                   rv32i_exec_xor);
    // RV32I_INST_DECLARE(rv32i_base_table, "srl",     RV32I_INST_TYPE_R, RV32I_OPCODE_CAL,    0b101, 0b0000000,   NULL,                   rv32i_exec_srl);
    // RV32I_INST_DECLARE(rv32i_base_table, "sra",     RV32I_INST_TYPE_R, RV32I_OPCODE_CAL,    0b101, 0b0100000,   NULL,                   rv32i_exec_sra);
    // RV32I_INST_DECLARE(rv32i_base_table, "or",      RV32I_INST_TYPE_R, RV32I_OPCODE_CAL,    0b110, 0b0000000,   NULL,                   rv32i_exec_or);
    // RV32I_INST_DECLARE(rv32i_base_table, "and",     RV32I_INST_TYPE_R, RV32I_OPCODE_CAL,    0b111, 0b0000000,   NULL,                   rv32i_exec_and);

    RV32I_INST_DECLARE(rv32i_base_table, "addi",    RV32I_INST_TYPE_I, RV32I_OPCODE_CAL_I,  0b000, 0,           rv32i_inst_get_i_imm,   rv32i_exec_addi);
    // RV32I_INST_DECLARE(rv32i_base_table, "slti",    RV32I_INST_TYPE_I, RV32I_OPCODE_CAL_I,  0b010, 0,           rv32i_inst_get_i_imm,   rv32i_exec_addi);
    // RV32I_INST_DECLARE(rv32i_base_table, "sltiu",   RV32I_INST_TYPE_I, RV32I_OPCODE_CAL_I,  0b011, 0,           rv32i_inst_get_i_imm,   rv32i_exec_addi);
    // RV32I_INST_DECLARE(rv32i_base_table, "xori",    RV32I_INST_TYPE_I, RV32I_OPCODE_CAL_I,  0b100, 0,           rv32i_inst_get_i_imm,   rv32i_exec_addi);
    // RV32I_INST_DECLARE(rv32i_base_table, "ori",     RV32I_INST_TYPE_I, RV32I_OPCODE_CAL_I,  0b110, 0,           rv32i_inst_get_i_imm,   rv32i_exec_addi);
    // RV32I_INST_DECLARE(rv32i_base_table, "andi",    RV32I_INST_TYPE_I, RV32I_OPCODE_CAL_I,  0b111, 0,           rv32i_inst_get_i_imm,   rv32i_exec_addi);
    // RV32I_INST_DECLARE(rv32i_base_table, "slli",    RV32I_INST_TYPE_I, RV32I_OPCODE_CAL_I,  0b001, 0b0000000,   rv32i_inst_get_i_imm,   rv32i_exec_addi);
    // RV32I_INST_DECLARE(rv32i_base_table, "srli",    RV32I_INST_TYPE_I, RV32I_OPCODE_CAL_I,  0b101, 0,           rv32i_inst_get_i_imm,   rv32i_exec_addi);
    // RV32I_INST_DECLARE(rv32i_base_table, "srai",    RV32I_INST_TYPE_I, RV32I_OPCODE_CAL_I,  0b101, 0,           rv32i_inst_get_i_imm,   rv32i_exec_addi);

    return;
}
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

typedef enum {
    RV32I_INST_TYPE_R,
    RV32I_INST_TYPE_I,
    RV32I_INST_TYPE_S,
    RV32I_INST_TYPE_B,
    RV32I_INST_TYPE_U,
    RV32I_INST_TYPE_J,
    RV32I_INST_TYPE_INVALID
} rv32i_inst_type_e;

/*
    按照 RV32I Base Instruction Set 顺序进行排列
    RV32I 的 opcode[1:0] = 2'b11
    同时结合 1.5.1. Expanded Instruction-Length Encoding
    RV32I的inst验证需要增加 inst[1:0] == 2'b11 & bbb!= 111的情况
    xxxxxxxxxxxbbb11
*/
typedef enum {
    RV32I_OPCODE_LUI = 0b01101,    //U x[rd] = sext(immediate[31:12] << 12)
    RV32I_OPCODE_AUIPC = 0b00101,  //U x[rd] = pc + sext(immediate[31:12] << 12)
    RV32I_OPCODE_JAL = 0b11011,    //J x[rd] = pc+4; pc += sext(offset)
    RV32I_OPCODE_JAL_R = 0b11001,  //I t=pc+4; pc=(x[rs1]+sext(offset))&∼1; x[rd]=t
    RV32I_OPCODE_B = 0b11000,      //B beq bne blt bge bltu bgeu
    RV32I_OPCODE_LOAD = 0b00000,   //I lb lh lw lbu lhu
    RV32I_OPCODE_S = 0b01000,      //S sb sh sw
    RV32I_OPCODE_CAL_I = 0b00100,  //I addi slti sltiu xori ori andi slli srli srai
    RV32I_OPCODE_CAL = 0b1100,     //R add sub sll slt sltu xor srl sra or and
    RV32I_OPCODE_FENCE = 0b00011,  //I fence fence.i
    RV32I_OPCODE_E_CSR = 0b11100,   //I E: ecall ebreak, CSR: csrrw csrrs csrrc csrrwi csrrsi csrrci
    RV32I_OPCODE_INVALID = 0b11111
} rv32i_opcode_e;

typedef struct {
    rv32i_inst_type_e type;
    uint8_t opcode;
    uint8_t funct3;
    uint8_t funct7;
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
    int32_t imm;
    uint32_t inst;
} rv32i_decoded_inst_t;

typedef uint32_t (*rv32i_inst_exec_fn_t)(rv32i_inst_t inst);

typedef struct {
    char rv32i_inst_name[20];
    rv32i_inst_type_e type;
    rv32i_opcode_e    opcode;
    uint8_t funct3;
    uint8_t funct7;
    rv32i_inst_exec_fn_t rv32i_inst_exec_fn;
} rv32i_base_inst_t;

#define RV32I_INST_DECLARE(table, name, ty, op, f3, f7, func_ptr) do { \
    int idx = (table).count; \
    if (idx < RV32I_MAX_INSTRUCTIONS) { \
        strncpy((table).instructions[idx].rv32i_inst_name, name, 19); \
        (table).instructions[idx].rv32i_inst_name[19] = '\0'; \
        (table).instructions[idx].type = ty; \
        (table).instructions[idx].opcode = op; \
        (table).instructions[idx].funct3 = f3; \
        (table).instructions[idx].funct7 = f7; \
        (table).instructions[idx].rv32i_inst_exec_fn = func_ptr; \
        (table).count++; \
    } \
} while(0)

/*
    in memory.c
    memory by rv32i_load_word -> inst

*/
uint32_t rv32i_opcode_fetch(rv32i_inst_t *inst);

/*
    in opcodes.c
    inst -> rv32i_decoded_inst_t ->寻找注册的指令表中是否有对应的
*/
uint32_t rv32i_opcode_decode(rv32i_inst_t inst, rv32i_decoded_inst_t *decoded_inst, rv32i_base_inst_t *base_inst);


/*
    暂定 opcodes.c
    调用base_inst的 exec_fn 运行对应指令操作
*/
uint32_t rv32i_opcode_execute(rv32i_inst_t inst , rv32i_base_inst_t base_inst);


uint32_t rv32i_exec_once();

void rv32i_init_instructions(void);

#endif  //__RV32I_INSTRUCTION_H__

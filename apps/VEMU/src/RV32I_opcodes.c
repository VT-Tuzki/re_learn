#include <stdio.h>
#include <string.h>

#include "RV32I_instruction.h"
#include "RV32I_memory.h"
#include "RV32I_registers.h"

#include "core/cc_stdint.h"
#include "ds/array/cc_array.h"


#define RV32I_MAX_INSTRUCTIONS 105

typedef struct {
    rv32i_base_inst_t instructions[RV32I_MAX_INSTRUCTIONS];
    uint32_t count;
} rv32i_base_table_t;


rv32i_base_table_t rv32i_base_table = {};

uint32_t rv32i_exec_add (rv32i_inst_t inst) {
    printf("This is rv32i add\n");
    return 0;
}

uint32_t rv32i_exec_sub (rv32i_inst_t inst) {
    printf("This is rv32i sub\n");
    return 0;
}

void rv32i_init_instructions(void) {

    RV32I_INST_DECLARE(rv32i_base_table, "add", RV32I_INST_TYPE_R, RV32I_OPCODE_CAL, 0b000, 0b0000000, rv32i_exec_add);
    // RV32I_INST_DECLARE(rv32i_base_table, "sub", RV32I_R, RV32I_CAL, 0b000, 0b0100000, rv32i_exec_sub);
    return;
}

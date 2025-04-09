#include "RV32I.h"
#include <stdio.h>

#include "core/cc_dbg.h"

#define MEMORY_SIZE (64 * 1024)
#define BASE_ADDR 0x1000
rv32i_memory_t run_memory = {0};


int main() {
    int res = 0;
    rv32i_reset_registers();
    rv32i_init_inst_table();
    res = rv32i_init_memory(&run_memory, MEMORY_SIZE, BASE_ADDR);
    check_res_ok(res, " rv32i_init_memory error. ");
    uint32_t program[] = {
        0x00500093,  // addi x1, x0, 5
        0x00A00113,  // addi x2, x0, 10
        0x002081B3,  // add x3, x1, x2
        0x40218233,  // sub x4, x3, x2
        0x004202B3   // add x5, x4, x4
    };

    res = rv32i_memory_load_block(&run_memory, 0x1000, (uint8_t*)program, sizeof(program));
    check_res_ok(res, " rv32i_memory_load_block error. ");

    printf("start progream.\n");
    res = rv32i_exec_run(&run_memory, 5, BASE_ADDR);
    check_res_ok(res, " rv32i_memory_load_block error. ");
    printf("end progream.\n");
    return 0;
error:
    return res;
}



#include <stdio.h>
#include <assert.h>

#include "RV32I.h"



#define MEMORY_SIZE (64 * 1024)
#define BASE_ADDR 0x1000
int main() {
    printf("Starting RISC-V add_sub tests...\n");

    rv32i_memory_t test_mem = {0};
    uint32_t res;

    // addi x1, x0, 5    # 设置x1 = 5    0101 00000 000 00001 0010011
    // addi x2, x0, 10   # 设置x2 = 10
    // add x3, x1, x2    # x3 = x1 + x2 = 15 0000000 00010 00001 000 00011 0110011
    // sub x4, x3, x2    # x4 = x3 - x2 = 5
    // add x5, x4, x4    # x5 = x4 + x4 = 10
    uint32_t program[] = {
        0x00500093,  // addi x1, x0, 5
        0x00A00113,  // addi x2, x0, 10
        0x002081B3,  // add x3, x1, x2
        0x40218233,  // sub x4, x3, x2
        0x004202B3   // add x5, x4, x4
    };
    rv32i_reset_registers();
    rv32i_init_inst_table();
    res = rv32i_init_memory(&test_mem, MEMORY_SIZE, BASE_ADDR);
    assert(res == 0);



    res = rv32i_memory_load_block(&test_mem, 0x1000, (uint8_t*)program, sizeof(program));
    assert(res == 0);

    printf("start\n");


    res = rv32i_exec_run(&test_mem, 5, BASE_ADDR);
    assert(res == 0);

    rv32i_print_reg();

    uint32_t value;
    rv32i_read_reg(1, &value);
    printf("验证 x1 = 5: %s\n", (value == 5) ? "通过" : "失败");

    rv32i_read_reg(2, &value);
    printf("验证 x2 = 10: %s\n", (value == 10) ? "通过" : "失败");

    rv32i_read_reg(3, &value);
    printf("验证 x3 = 15: %s\n", (value == 15) ? "通过" : "失败");

    rv32i_read_reg(4, &value);
    printf("验证 x4 = 5: %s\n", (value == 5) ? "通过" : "失败");

    rv32i_read_reg(5, &value);
    printf("验证 x5 = 10: %s\n", (value == 10) ? "通过" : "失败");

    res = rv32i_free_memory(&test_mem);
    assert(res == 0);
    printf("rv32i block memory test ok\n");

    printf("All add_sub tests passed!\n");
    return 0;
}
#include <stdio.h>
#include <assert.h>

#include "RV32I.h"

void test_register_operations() {
    uint32_t value;

    rv32i_reset_registers();

    rv32i_read_reg(0, &value);
    assert(value == 0);

    rv32i_write_reg(0, 0xDEADBEEF);
    rv32i_read_reg(0, &value);
    assert(value == 0);

    for (int i = 1; i < 32; i++) {
        rv32i_write_reg(i, i * 0x100);
        rv32i_read_reg(i, &value);
        assert(value == i * 0x100);
    }

    rv32i_set_pc(0x1000);
    rv32i_read_pc(&value);
    assert(value == 0x1000);

    rv32i_set_pc(0x1008);
    rv32i_read_pc(&value);
    assert(value == 0x1008);
    rv32i_print_reg();

    printf("All register tests passed!\n");
}

int main() {
    test_register_operations();
    return 0;
}
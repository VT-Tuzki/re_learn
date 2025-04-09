#include "RV32I.h"
#include <stdio.h>


int main() {
    int res = 0;
    rv32i_reset_registers();
    rv32i_init_inst_table();

    printf("test1\n");
    rv32i_print_reg();
    return 0;
}



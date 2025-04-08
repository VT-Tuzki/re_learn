#include "RV32I_registers.h"
#include <stdio.h>


int main() {
    int res = 0;
    rv32i_reset_registers();
    printf("test1\n");
    rv32i_print_reg();
    return 0;
}



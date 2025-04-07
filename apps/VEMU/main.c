#include "RISC_V_integer_reg.h"
#include <stdio.h>


int main() {
    int res = 0;
    rv32i_reset_registers();
    printf("test1\n");
    rv32i_print_reg();
    return 0;
}



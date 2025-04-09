#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "RV32I_instruction.h"
#include "RV32I_memory.h"
#include "RV32I_registers.h"

#include "core/cc_stdint.h"
#include "ds/array/cc_array.h"


/*
2.3. Immediate Encoding Variants

*/
uint32_t rv32i_inst_get_i_imm(rv32i_inst_t inst, uint32_t *imm) {
    int32_t i_imm = (int32_t)(inst.rv32i_itype.imm11_0);
    printf("i_imm%d\n",i_imm);
    //inst[31]
    if (i_imm & 0x800) {
        i_imm |= 0xFFFFF000;
    }

    *imm = i_imm;
    return 0;
}

uint32_t rv32i_inst_get_s_imm(rv32i_inst_t inst, uint32_t *imm) {
    int32_t s_imm = (inst.rv32i_stype.imm11_5 << 5) | inst.rv32i_stype.imm4_0;

    if(s_imm & 0x800) {
        s_imm |= 0xFFFFF000;
    }

    *imm = s_imm;
    return 0;
}

uint32_t rv32i_inst_get_b_imm(rv32i_inst_t inst, uint32_t *imm) {
    int32_t b_imm = (inst.rv32i_btype.imm12 << 12) |
                        (inst.rv32i_btype.imm11 << 11) |
                        (inst.rv32i_btype.imm10_5 << 5) |
                        (inst.rv32i_btype.imm4_1 << 1);

    if(b_imm & 0x1000) {
        b_imm |= 0xFFFFE000;
    }
    *imm = b_imm;
    return 0;
}

uint32_t rv32i_inst_get_u_imm(rv32i_inst_t inst, uint32_t *imm) {
    int32_t u_imm = (inst.rv32i_utype.imm31_12 << 12);

    *imm = u_imm;
    return 0;
}

uint32_t rv32i_inst_get_j_imm(rv32i_inst_t inst, uint32_t *imm) {
    int32_t j_imm = (inst.rv32i_jtype.imm20 << 20) |
                        (inst.rv32i_jtype.imm19_12 << 12) |
                        (inst.rv32i_jtype.imm11 << 11) |
                        (inst.rv32i_jtype.imm10_1 << 1);

    if(j_imm & 0x100000) {
        j_imm |= 0xFFE00000;
    }
    *imm = j_imm;
    return 0;
}


uint32_t rv32i_opcode_fetch(rv32i_memory_t *rv32i_memory, rv32i_inst_t *inst) {

    uint32_t pc;
    rv32i_read_pc(&pc);

    uint32_t new_inst;
    uint32_t res = rv32i_load_word(rv32i_memory, pc, &new_inst);
    if(res != 0) return res;
    *(uint32_t*) inst = new_inst;
    return 0;
}

/*
    in opcodes.c
    inst -> rv32i_decoded_inst_t ->寻找注册的指令表中是否有对应的
*/
uint32_t rv32i_opcode_decode(rv32i_memory_t *rv32i_memory, rv32i_inst_t inst, rv32i_decoded_inst_t *decoded_inst, rv32i_base_inst_t *base_inst) {
    uint32_t res = rv32i_inst_table_find(inst, decoded_inst, base_inst);
    if(res != 0) return res;


    return 0;
}


/*
    暂定 opcodes.c
    调用base_inst的 exec_fn 运行对应指令操作
*/
uint32_t rv32i_opcode_execute(rv32i_memory_t *rv32i_memory, rv32i_inst_t inst , rv32i_decoded_inst_t decoded_inst, rv32i_base_inst_t base_inst) {
    if (base_inst.rv32i_inst_exec_fn == NULL) {
        return 1;
    }

    return base_inst.rv32i_inst_exec_fn(decoded_inst);
}


uint32_t rv32i_exec_once(rv32i_memory_t *rv32i_memory) {
    rv32i_inst_t inst;
    rv32i_decoded_inst_t decoded_inst;
    rv32i_base_inst_t base_inst;
    uint32_t res;
    uint32_t old_pc;

    rv32i_read_pc(&old_pc);

    res = rv32i_opcode_fetch(rv32i_memory, &inst);
    if(res != 0) {
        printf("rv32i_opcode_fetch error\n");
        return res;
    }
    printf("get: inst: %u\n", *(uint32_t*)&inst);
    res = rv32i_opcode_decode(rv32i_memory, inst, &decoded_inst, &base_inst);
    if(res != 0) {
        printf("rv32i_opcode_decode error\n");
        return res;
    }

    res = rv32i_opcode_execute(rv32i_memory, inst, decoded_inst, base_inst);
    if(res != 0) {
        printf("rv32i_opcode_execute error\n");
        return res;
    }

    uint32_t new_pc;
    rv32i_read_pc(&new_pc);
    if(new_pc == old_pc) {
        rv32i_set_pc(old_pc + 4);
    }

    return 0;
}


uint32_t rv32i_exec_run(rv32i_memory_t *rv32i_memory, uint32_t step, uint32_t addr) {
    uint32_t now_run_over_step = 0;
    uint32_t res = 0;

    rv32i_set_pc(addr);

    while(now_run_over_step < step) {
        res = rv32i_exec_once(rv32i_memory);
        rv32i_print_reg();
        if(res != 0) {
            printf("run error :%d\n",res);
            return res;
        }

        now_run_over_step++;
    }
    printf("执行完毕\n");
    return 0;
}


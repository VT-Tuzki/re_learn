#ifndef __RV32I_MEMORY_H__
#define __RV32I_MEMORY_H__

#include "core/cc_stdint.h"

typedef struct {
    uint8_t *data;
    uint32_t size;
    uint32_t base_addr;
} rv32i_memory_t;

extern rv32i_memory_t rv32i_memory;

typedef enum {
    RV32I_MEMORY_OK = 0,
    RV32I_MEMORY_ADDR_NOALIGNED,
    RV32I_MEMORY_ADDR_OUT,
    RV32I_MEMORY_NULL
}rv32i_memory_status_e;


/*

*/
uint32_t rv32i_init_memory(rv32i_memory_t *rv32i_memory, uint32_t size, uint32_t base_addr);
uint32_t rv32i_free_memory(rv32i_memory_t *rv32i_memory);
uint32_t rv32i_reset_memory(rv32i_memory_t *rv32i_memory);

uint32_t rv32i_memory_load_block(rv32i_memory_t *rv32i_memory, uint32_t addr, uint8_t *data, uint32_t size);
uint32_t rv32i_memory_load_progream(rv32i_memory_t *rv32i_memory, uint32_t addr, char *filename);
uint32_t rv32i_memory_print(rv32i_memory_t *rv32i_memory, uint32_t addr, uint32_t size);


uint32_t rv32i_load_byte(rv32i_memory_t *rv32i_memory, uint32_t addr, uint8_t *value);
uint32_t rv32i_store_byte(rv32i_memory_t *rv32i_memory, uint32_t addr, uint8_t value);

uint32_t rv32i_load_halfword(rv32i_memory_t *rv32i_memory, uint32_t addr, uint16_t *value);
uint32_t rv32i_store_halfword(rv32i_memory_t *rv32i_memory, uint32_t addr, uint16_t value);

uint32_t rv32i_load_word(rv32i_memory_t *rv32i_memory, uint32_t addr, uint32_t *value);
uint32_t rv32i_store_word(rv32i_memory_t *rv32i_memory, uint32_t addr, uint32_t value);




/*
    from RISC-V Volume I
    1.4 memory
    可以读取和存储的大小
    RISC-V:
        byte   8bit   1byte
        word ->32 bit 4byte
        halfword 16bit 2byte
        doubleword 64bit 8byte
        quadword 128bit 16byte

    寻址范围
        地址0 与 2xlen次方-1地址相邻 实现时进行取模
    但RV32I 从load 和store 2.6 以及立即数2.3 来看 只需要实现byte word halfword


*/

#endif // __RV32I_MEMORY_H__
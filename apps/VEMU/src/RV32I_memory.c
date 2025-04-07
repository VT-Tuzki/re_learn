#include <string.h>


#include "RV32I_memory.h"



#define MEMORY_SIZE (4*1024*1024)
static uint8_t memory[MEMORY_SIZE] __attribute__((aligned(8)));


uint32_t rv32i_init_memory() {

    memset(memory, 0, MEMORY_SIZE);

    return 0;
}

uint32_t rv32i_load_byte(uint32_t addr, uint8_t *value) {
    if(value == NULL) {
        return 1;
    }

    addr = addr%MEMORY_SIZE;

    *value = memory[addr];

    return 0;
}
uint32_t rv32i_store_byte(uint32_t addr, uint8_t value) {
    addr = addr%MEMORY_SIZE;
    memory[addr] = value;
    return 0;
}

uint32_t rv32i_load_halfword(uint32_t addr, uint16_t *value) {
    if(addr & 1 ) return 2;

    addr = addr % MEMORY_SIZE;

    *value = (uint16_t)memory[addr] | memory[(addr + 1) % MEMORY_SIZE];

    return 0;
}
uint32_t rv32i_store_halfword(uint32_t addr, uint16_t value) {
    if(addr & 1 ) return 2;
    addr = addr % MEMORY_SIZE;

    memory[addr] = (uint8_t)(value&0xFF);
    memory[(addr + 1) % MEMORY_SIZE] = (uint8_t)((value>>8)&0xFF);
    return 0;
}

uint32_t rv32i_load_word(uint32_t addr, uint32_t *value) {
    if(addr % 4 != 0) return 2;

    addr = addr % MEMORY_SIZE;

    *value = (uint32_t)memory[addr] |
             ((uint32_t)memory[(addr + 1) % MEMORY_SIZE] << 8) |
             ((uint32_t)memory[(addr + 2) % MEMORY_SIZE] << 16) |
             ((uint32_t)memory[(addr + 3) % MEMORY_SIZE] << 24);

    return 0;
}
uint32_t rv32i_store_word(uint32_t addr, uint32_t value) {
    if(addr % 4 != 0) return 2;

    memory[addr] = (uint8_t)(value & 0xFF);
    memory[(addr + 1) % MEMORY_SIZE] = (uint8_t)((value >> 8) & 0xFF);
    memory[(addr + 2) % MEMORY_SIZE] = (uint8_t)((value >> 16) & 0xFF);
    memory[(addr + 3) % MEMORY_SIZE] = (uint8_t)((value >> 24) & 0xFF);

    return 0;
}
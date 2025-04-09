#include <string.h>
#include <stdio.h>
#include "core/cc_mem.h"
#include "RV32I_memory.h"



rv32i_memory_t rv32i_memory = {0};

uint32_t rv32i_init_memory(rv32i_memory_t *rv32i_memory, uint32_t size, uint32_t base_addr) {
    rv32i_memory->data = (uint8_t*) calloc(size, 1);
    if(rv32i_memory->data == NULL) {
        return RV32I_MEMORY_NULL;
    }
    rv32i_memory->size = size;
    rv32i_memory->base_addr = base_addr;
    return 0;
}

uint32_t rv32i_free_memory(rv32i_memory_t *rv32i_memory) {
    if (rv32i_memory->data != NULL) {
        free(rv32i_memory->data);
        rv32i_memory->data = NULL;
    }
    rv32i_memory->size = 0;
    rv32i_memory->base_addr = 0;
    return 0;
}

uint32_t rv32i_reset_memory(rv32i_memory_t *rv32i_memory) {
    if (rv32i_memory->data != NULL && rv32i_memory->size > 0) {
        memset(rv32i_memory->data, 0, rv32i_memory->size);
    }
    return 0;
}

uint32_t rv32i_memory_load_block(rv32i_memory_t *rv32i_memory, uint32_t addr, uint8_t *data, uint32_t size) {
    if(data == NULL || rv32i_memory->data == NULL) {
        return RV32I_MEMORY_NULL;
    }

    if (addr + size > rv32i_memory->size) {
        return RV32I_MEMORY_ADDR_OUT;
    }

    memcpy(rv32i_memory->data + addr, data, size);

    return 0;
}

uint32_t rv32i_memory_load_progream(rv32i_memory_t *rv32i_memory, uint32_t addr, char *filename) {
    //TODO:待实现 暂时不加载文件

    return 0;
}

uint32_t rv32i_memory_print(rv32i_memory_t *rv32i_memory, uint32_t addr, uint32_t size) {
    if (rv32i_memory->data == NULL) {
        return RV32I_MEMORY_NULL;
    }

    if (addr + size > rv32i_memory->size) {
        return RV32I_MEMORY_ADDR_OUT;
    }

    printf("Memory print from 0x%08x to 0x%08x:\n", addr, addr + size - 1);

    for (uint32_t i = 0; i < size; i++) {
        if (i % 16 == 0) {
            printf("\n0x%08x: ", addr + i);
        }
        printf("%02x ", rv32i_memory->data[addr + i]);
    }
    printf("\n");

    return 0;
}





uint32_t rv32i_load_byte(rv32i_memory_t *rv32i_memory, uint32_t addr, uint8_t *value) {
    if(value == NULL || rv32i_memory->data == NULL) {
        return RV32I_MEMORY_NULL;
    }

    addr = addr % rv32i_memory->size;

    *value = rv32i_memory->data[addr];

    return 0;
}
uint32_t rv32i_store_byte(rv32i_memory_t *rv32i_memory, uint32_t addr, uint8_t value) {
    if(rv32i_memory->data == NULL) {
        return RV32I_MEMORY_NULL;
    }

    addr = addr % rv32i_memory->size;
    rv32i_memory->data[addr] = value;
    return 0;
}

uint32_t rv32i_load_halfword(rv32i_memory_t *rv32i_memory, uint32_t addr, uint16_t *value) {
    if(value == NULL || rv32i_memory->data == NULL) {
        return RV32I_MEMORY_NULL;
    }

    if(addr & 1 ) return RV32I_MEMORY_ADDR_NOALIGNED;

    addr = addr % rv32i_memory->size;

    *value = (uint16_t)rv32i_memory->data[addr] | ((uint16_t)rv32i_memory->data[(addr + 1) % rv32i_memory->size] << 8);

    return 0;
}
uint32_t rv32i_store_halfword(rv32i_memory_t *rv32i_memory, uint32_t addr, uint16_t value) {
    if(rv32i_memory->data == NULL) {
        return RV32I_MEMORY_NULL;
    }

    if(addr & 1 ) return RV32I_MEMORY_ADDR_NOALIGNED;

    addr = addr % rv32i_memory->size;

    rv32i_memory->data[addr] = (uint8_t)(value&0xFF);
    rv32i_memory->data[(addr + 1) % rv32i_memory->size] = (uint8_t)((value>>8)&0xFF);
    return 0;
}

uint32_t rv32i_load_word(rv32i_memory_t *rv32i_memory, uint32_t addr, uint32_t *value) {
    if(value == NULL || rv32i_memory->data == NULL) {
        return RV32I_MEMORY_NULL;
    }

    if(addr % 4 != 0) return RV32I_MEMORY_ADDR_NOALIGNED;

    addr = addr % rv32i_memory->size;

    *value = (uint32_t)rv32i_memory->data[addr] |
             ((uint32_t)rv32i_memory->data[(addr + 1) % rv32i_memory->size] << 8) |
             ((uint32_t)rv32i_memory->data[(addr + 2) % rv32i_memory->size] << 16) |
             ((uint32_t)rv32i_memory->data[(addr + 3) % rv32i_memory->size] << 24);

    return 0;
}
uint32_t rv32i_store_word(rv32i_memory_t *rv32i_memory, uint32_t addr, uint32_t value) {
    if(rv32i_memory->data == NULL) {
        return RV32I_MEMORY_NULL;
    }

    if(addr % 4 != 0) return RV32I_MEMORY_ADDR_NOALIGNED;

    addr = addr % rv32i_memory->size;


    rv32i_memory->data[addr] = (uint8_t)(value & 0xFF);
    rv32i_memory->data[(addr + 1) % rv32i_memory->size] = (uint8_t)((value >> 8) & 0xFF);
    rv32i_memory->data[(addr + 2) % rv32i_memory->size] = (uint8_t)((value >> 16) & 0xFF);
    rv32i_memory->data[(addr + 3) % rv32i_memory->size] = (uint8_t)((value >> 24) & 0xFF);

    return 0;
}
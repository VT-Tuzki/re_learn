#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "RV32I_memory.h"

#define MEMORY_SIZE (64 * 1024)
#define BASE_ADDR 0x10000000
/*
void test_memory_() {
    rv32i_memory_t test_mem = {0};
    uint32_t res;

    res = rv32i_init_memory(&test_mem, MEMORY_SIZE, BASE_ADDR);
    assert(res == 0);


    res = rv32i_free_memory(&test_mem);
    assert(res == 0);
    printf("rv32i   memory test ok\n");
    return;
}
*/
void test_memory_init_free() {
    rv32i_memory_t test_mem = {0};
    uint32_t res;

    res = rv32i_init_memory(&test_mem, MEMORY_SIZE, BASE_ADDR);
    assert(res == 0);
    assert(test_mem.data != NULL);
    assert(test_mem.size == MEMORY_SIZE);
    assert(test_mem.base_addr == BASE_ADDR);

    res = rv32i_free_memory(&test_mem);
    assert(res == 0);
    assert(test_mem.data == NULL);
    assert(test_mem.size == 0);
    assert(test_mem.base_addr == 0);

    res = rv32i_free_memory(&test_mem);
    assert(res == 0);
    printf("rv32i init free  memory test ok\n");
    return;
}

void test_memory_reset() {
    rv32i_memory_t test_mem = {0};
    uint32_t res;

    res = rv32i_init_memory(&test_mem, MEMORY_SIZE, BASE_ADDR);
    assert(res == 0);

    memset(test_mem.data, 0xAA, MEMORY_SIZE);

    res = rv32i_reset_memory(&test_mem);
    assert(res == 0);

    for(int i = 0; i < MEMORY_SIZE; i++) {
        assert(test_mem.data[i] == 0);
    }


    res = rv32i_free_memory(&test_mem);
    assert(res == 0);
    printf("rv32i reset memory test ok\n");
    return;
}

void test_memory_byte() {
    rv32i_memory_t test_mem = {0};
    uint32_t res;

    res = rv32i_init_memory(&test_mem, MEMORY_SIZE, BASE_ADDR);
    assert(res == 0);

    for(int i = 0; i < 256; i++) {
        res = rv32i_store_byte(&test_mem, i, i);
        assert(res == 0);
    }

    uint8_t value;
    for(int i = 0; i < 256; i++) {
        res = rv32i_load_byte(&test_mem, i, &value);
        assert(res == 0);
        assert(value == i);
    }

    res = rv32i_store_byte(&test_mem, MEMORY_SIZE + 10, 0xAA);
    assert(res == 0);
    res = rv32i_load_byte(&test_mem, 10, &value);
    assert(res == 0);
    assert(value == 0xAA);


    res = rv32i_free_memory(&test_mem);
    assert(res == 0);
    printf("rv32i byte  memory test ok\n");
    return;
}

void test_memory_halfword() {
    rv32i_memory_t test_mem = {0};
    uint32_t res;

    res = rv32i_init_memory(&test_mem, MEMORY_SIZE, BASE_ADDR);
    assert(res == 0);

    for(int i = 0; i < 256; i++) {
        res = rv32i_store_halfword(&test_mem, i*2, i*0xAA);
        assert(res == 0);
    }

    uint16_t value;
    for(int i = 0; i < 256; i++) {
        res = rv32i_load_halfword(&test_mem, i*2, &value);
        assert(res == 0);
        assert(value == i*0xAA);
    }

    res = rv32i_store_halfword(&test_mem, 1, 0xAA55);
    assert(res == RV32I_MEMORY_ADDR_NOALIGNED);
    res = rv32i_load_halfword(&test_mem, 1, &value);
    assert(res == RV32I_MEMORY_ADDR_NOALIGNED);

    res = rv32i_store_halfword(&test_mem, MEMORY_SIZE + 10, 0xAA55);
    assert(res == 0);
    res = rv32i_load_halfword(&test_mem, 10, &value);
    assert(res == 0);
    assert(value == 0xAA55);

    res = rv32i_free_memory(&test_mem);
    assert(res == 0);
    printf("rv32i halfword  memory test ok\n");
    return;
}

void test_memory_word() {
    rv32i_memory_t test_mem = {0};
    uint32_t res;

    res = rv32i_init_memory(&test_mem, MEMORY_SIZE, BASE_ADDR);
    assert(res == 0);

    for(int i = 0; i < 256; i++) {
        res = rv32i_store_word(&test_mem, i*4, i*0xAA);
        assert(res == 0);
    }

    uint32_t value;
    for(int i = 0; i < 256; i++) {
        res = rv32i_load_word(&test_mem, i*4, &value);
        assert(res == 0);
        assert(value == i*0xAA);
    }

    res = rv32i_store_word(&test_mem, 1, 0xAA5555AA);
    assert(res == RV32I_MEMORY_ADDR_NOALIGNED);
    res = rv32i_load_word(&test_mem, 1, &value);
    assert(res == RV32I_MEMORY_ADDR_NOALIGNED);

    res = rv32i_store_word(&test_mem, 2, 0xAA5555AA);
    assert(res == RV32I_MEMORY_ADDR_NOALIGNED);
    res = rv32i_load_word(&test_mem, 2, &value);
    assert(res == RV32I_MEMORY_ADDR_NOALIGNED);

    res = rv32i_store_word(&test_mem, MEMORY_SIZE + 12, 0xAA5555AA);
    assert(res == 0);
    res = rv32i_load_word(&test_mem, 12, &value);
    assert(res == 0);
    assert(value == 0xAA5555AA);

    res = rv32i_free_memory(&test_mem);
    assert(res == 0);
    printf("rv32i word  memory test ok\n");
    return;
}

void test_memory_block() {
    rv32i_memory_t test_mem = {0};
    uint32_t res;
    uint8_t test_block[256];

    res = rv32i_init_memory(&test_mem, MEMORY_SIZE, BASE_ADDR);
    assert(res == 0);

    for (int i = 0; i < 256; i++) {
        test_block[i] = (uint8_t)i;
    }

    res = rv32i_memory_load_block(&test_mem, 0x1000, test_block, 256);
    assert(res == 0);

    for (int i = 0; i < 256; i++) {
        uint8_t value;
        res = rv32i_load_byte(&test_mem, 0x1000 + i, &value);
        assert(res == 0);
        assert(value == (uint8_t)i);
    }

    res = rv32i_memory_load_block(&test_mem, MEMORY_SIZE - 10, test_block, 256);
    assert(res == RV32I_MEMORY_ADDR_OUT);

    res = rv32i_free_memory(&test_mem);
    assert(res == 0);
    printf("rv32i block memory test ok\n");
    return;
}

void test_memory_print() {
    rv32i_memory_t test_mem = {0};
    uint32_t res;
    uint8_t test_block[256];

    res = rv32i_init_memory(&test_mem, MEMORY_SIZE, BASE_ADDR);
    assert(res == 0);

    for (int i = 0; i < 256; i++) {
        test_block[i] = (uint8_t)i;
    }

    res = rv32i_memory_load_block(&test_mem, 0x1000, test_block, 256);
    assert(res == 0);

    printf("Testing memory print function:\n");
    res = rv32i_memory_print(&test_mem, 0x1000, 256);
    assert(res == 0);

    res = rv32i_free_memory(&test_mem);
    assert(res == 0);
    printf("rv32i print memory test ok\n");
    return;
}


int main() {
    printf("Starting RISC-V memory tests...\n");

    test_memory_init_free();
    test_memory_reset();
    test_memory_byte();
    test_memory_halfword();
    test_memory_word();
    test_memory_block();
    test_memory_print();

    printf("All memory tests passed!\n");
    return 0;
}
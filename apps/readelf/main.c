/**
 * @file main.c
 * @brief Main entry point for template app
 *
 * This file contains the main function that serves as the entry point
 * for the template application.
 */

#include "read_elf.h"

/**
 * @brief Application entry point
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Exit code
 */
int main(int argc, char* argv[]) {
    return read_elf_main(argc, argv);
}
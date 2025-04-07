/**
 * @file template_app.h
 * @brief Public interface for template app library
 */

#ifndef TEMPLATE_APP_H
#define TEMPLATE_APP_H

/**
 * @brief Add two integers
 *
 * @param a First integer
 * @param b Second integer
 * @return Sum of a and b
 */
int template_app_add(int a, int b);

/**
 * @brief Main function for template app
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Exit code
 */
int template_app_main(int argc, char* argv[]);

#endif /* TEMPLATE_APP_H */

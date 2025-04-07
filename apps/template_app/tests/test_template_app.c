/**
 * @file test_template_app.c
 * @brief Tests for template app functions
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmockery.h>
#include "template_app.h"

/* Test the add function */
static void test_add(void **state) {
    assert_int_equal(template_app_add(5, 7), 12);
    assert_int_equal(template_app_add(-3, 3), 0);
    assert_int_equal(template_app_add(0, 0), 0);
}

/* Main test function */
int main(void) {
    const UnitTest tests[] = {
        unit_test(test_add),
    };
    return run_tests(tests);
}

# Tests for the template app

set(TEST_NAME test_${APP_NAME})

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/external/cmockery/include
)

# Define test source files
set(TEST_SOURCES
    test_template_app.c
    ../src/template_app.c
)

# Create test executable
add_executable(${TEST_NAME} ${TEST_SOURCES})

# Link against cmockery
target_link_libraries(${TEST_NAME} cmockery)

# Add test to CTest
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

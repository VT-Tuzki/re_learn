# Tests for lib_vtuzki_test

# Create a test executable
add_executable(test_s_array
    test_s_array.c
)

# Set its output directory
set_target_properties(test_s_array PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${APP_TEST_OUTPUT_DIR}"
)

# Include necessary directories
target_include_directories(test_s_array PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Link with external libraries as needed
target_use_external(test_s_array LIBRARIES vtuzkicc)

# Register with CTest - USE EXPLICIT NAMING
add_test(
    NAME "${APP_TEST_NAME}_test_s_array"
    COMMAND test_s_array
    WORKING_DIRECTORY "${APP_TEST_OUTPUT_DIR}"
)

# Print confirmation message for debugging
message(STATUS "Registered test: ${APP_TEST_NAME}_test_s_array in ${APP_TEST_OUTPUT_DIR}")
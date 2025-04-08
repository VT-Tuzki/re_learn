# Tests for VEMU

add_executable(test_rv32i_registers
    test_registers.c
)

target_link_libraries(test_rv32i_registers
    PRIVATE
        RV32I
        vtuzkicc
)

# Set its output directory
set_target_properties(test_rv32i_registers PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${APP_TEST_OUTPUT_DIR}"
)

# Include necessary directories
target_include_directories(test_rv32i_registers PRIVATE
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link with external libraries as needed
target_use_external(test_rv32i_registers LIBRARIES vtuzkicc)

# Register with CTest - USE EXPLICIT NAMING
add_test(
    NAME "${APP_TEST_NAME}_test_rv32i_registers"
    COMMAND test_rv32i_registers
    WORKING_DIRECTORY "${APP_TEST_OUTPUT_DIR}"
)

# Print confirmation message for debugging
message(STATUS "Registered test: ${APP_TEST_NAME}_test_rv32i_registers in ${APP_TEST_OUTPUT_DIR}")
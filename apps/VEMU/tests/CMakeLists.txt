# Tests for VEMU

function(add_vemu_test TEST_NAME TEST_SOURCE)
    # Create the executable
    add_executable(${TEST_NAME}
        ${TEST_SOURCE}
    )

    # Link common libraries
    target_link_libraries(${TEST_NAME}
        PRIVATE
            RV32I
            vtuzkicc
    )

    # Set output directory
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${APP_TEST_OUTPUT_DIR}"
    )

    # Include necessary directories
    target_include_directories(${TEST_NAME}
        PUBLIC
            ${CMAKE_SOURCE_DIR}/include
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    # Link with external libraries
    target_use_external(${TEST_NAME} LIBRARIES vtuzkicc)

    # Register with CTest
    add_test(
        NAME "${APP_TEST_NAME}_${TEST_NAME}"
        COMMAND ${TEST_NAME}
        WORKING_DIRECTORY "${APP_TEST_OUTPUT_DIR}"
    )

    # Print confirmation message
    message(STATUS "Registered test: ${APP_TEST_NAME}_${TEST_NAME} in ${APP_TEST_OUTPUT_DIR}")
endfunction()

add_vemu_test(test_rv32i_registers test_registers.c)
add_vemu_test(test_rv32i_memory test_memory.c)
cmake_minimum_required(VERSION 4.0)
project(re_learn VERSION 0.1.0 LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

option(RE_LEARN_BUILD_APPS "Build applications" ON)
option(RE_LEARN_BUILD_TESTS "Build tests" ON)

# New option: Specify apps to build (semicolon-separated list)
# Empty string means build all apps
set(RE_LEARN_SELECTED_APPS "" CACHE STRING "Specific apps to build (semicolon or comma separated, empty for all)")

# Enable testing if tests are enabled
if(RE_LEARN_BUILD_TESTS)
    enable_testing()
endif()

# Create apps directory in build folder
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/apps)

include(cmake/app_utils.cmake)

# Add dependencies first
add_subdirectory(external)
add_subdirectory(common)

# Then add applications
if(RE_LEARN_BUILD_APPS)
    add_subdirectory(apps)
endif()

# Optionally keep project-wide tests
if(RE_LEARN_BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()

if(RE_LEARN_BUILD_TESTS)
    # Get a list of all tests
    get_property(TEST_NAMES DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY TESTS)
    message(STATUS "Registered tests:")
    foreach(TEST_NAME ${TEST_NAMES})
        message(STATUS "  - ${TEST_NAME}")
    endforeach()
endif()

message(STATUS "")
message(STATUS "=== Build Configuration ===")
message(STATUS "Build applications: ${RE_LEARN_BUILD_APPS}")
message(STATUS "Build tests: ${RE_LEARN_BUILD_TESTS}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")
if(RE_LEARN_BUILD_APPS)
    if("${RE_LEARN_SELECTED_APPS}" STREQUAL "")
        message(STATUS "Building all apps")
        set(APPS_TO_BUILD "ALL" CACHE INTERNAL "Build all apps")
    else()
        message(STATUS "Selected apps: ${RE_LEARN_SELECTED_APPS}")
        # Convert comma-separated list to semicolon-separated if needed
        string(REPLACE "," ";" APPS_LIST "${RE_LEARN_SELECTED_APPS}")
        set(APPS_TO_BUILD "${APPS_LIST}" CACHE INTERNAL "Apps selected for building")
    endif()
endif()
message(STATUS "")

if(RE_LEARN_BUILD_TESTS)
    message(STATUS "=== Testing Information ===")
    message(STATUS "Run all tests:             ctest")
    message(STATUS "Run tests for a specific app: ctest -R <app_name>")
    message(STATUS "Examples:")
    message(STATUS "  - Run lib_vtuzki_test tests: ctest -R lib_vtuzki_test")
    message(STATUS "  - Run specific test:         ctest -R lib_vtuzki_test_test_s_array")
    message(STATUS "")
endif()
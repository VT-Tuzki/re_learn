# Helper functions for app configuration

# Function to check the required CMake version
function(check_cmake_version REQUIRED_VERSION)
    if(CMAKE_VERSION VERSION_LESS ${REQUIRED_VERSION})
        message(FATAL_ERROR "CMake version ${REQUIRED_VERSION} or higher is required. You are running ${CMAKE_VERSION}")
    endif()
endfunction()

# Check CMake version
check_cmake_version("4.0")

# Function to initialize external libraries table
function(initialize_external_libs)
    # Get all subdirectories in the external folder
    file(GLOB external_dirs RELATIVE "${CMAKE_SOURCE_DIR}/external" "${CMAKE_SOURCE_DIR}/external/*")

    message(STATUS "Found external libraries: ${external_dirs}")

    # Create options for each external library (OFF by default)
    foreach(lib ${external_dirs})
        if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/external/${lib}")
            option(USE_EXTERNAL_${lib} "Use external library: ${lib}" OFF)
            message(STATUS "External library ${lib} is ${USE_EXTERNAL_${lib}}")
        endif()
    endforeach()
endfunction()

# Call this function to initialize the table
initialize_external_libs()

# Function to add external dependencies to an app
function(target_use_external TARGET)
    # Parse the list of libraries to include
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs LIBRARIES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Process each requested library
    foreach(lib ${ARG_LIBRARIES})
        # Enable this library
        if(DEFINED USE_EXTERNAL_${lib})
            set(USE_EXTERNAL_${lib} OFF CACHE BOOL "Use external library: ${lib}" FORCE)
            message(STATUS "Enabling external library: ${lib} for ${TARGET}")

            # Add the subdirectory if not already added
            if(NOT TARGET ${lib})
                add_subdirectory(${CMAKE_SOURCE_DIR}/external/${lib} ${CMAKE_BINARY_DIR}/external/${lib})
            endif()

            # Link against the library
            target_link_libraries(${TARGET} PRIVATE ${lib})
        elseif(lib STREQUAL "vtuzkicc")
            # Special case for vtuzkicc which uses namespaced target
            target_link_libraries(${TARGET} PRIVATE vtuzkicc::vtuzkicc)
            message(STATUS "App ${TARGET} will use vtuzkicc")
        else()
            message(WARNING "Unknown external library: ${lib}")
        endif()
    endforeach()
endfunction()

# Function to set up an app with its own output directory
function(setup_app APP_NAME)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs SOURCES EXTERNAL_LIBS)
    cmake_parse_arguments(APP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Create the executable
    add_executable(${APP_NAME} ${APP_SOURCES})

    # Create app directory in build folder
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/apps/${APP_NAME})

    # Set output directory to be a subdirectory with the app's name
    set_target_properties(${APP_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/apps/${APP_NAME}"
    )

    # Include this app's include directory
    target_include_directories(${APP_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    # Link external libraries if specified
    if(APP_EXTERNAL_LIBS)
        target_use_external(${APP_NAME} LIBRARIES ${APP_EXTERNAL_LIBS})
    endif()

    # Link with common library if needed
    if(TARGET common_lib)
        target_link_libraries(${APP_NAME} PRIVATE common_lib)
    endif()

    # Install rules
    install(TARGETS ${APP_NAME}
        RUNTIME DESTINATION bin
    )

    # Message for clarity
    message(STATUS "App ${APP_NAME} will be built in ${CMAKE_BINARY_DIR}/apps/${APP_NAME}")
endfunction()

# Function to set up tests for an app
function(setup_app_tests APP_NAME)
    # Parse arguments
    set(options "")
    set(oneValueArgs TEST_DIR)
    set(multiValueArgs "")
    cmake_parse_arguments(APP_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Default test directory if not specified
    if(NOT APP_TEST_TEST_DIR)
        set(APP_TEST_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    endif()

    # Only proceed if tests directory exists
    if(EXISTS ${APP_TEST_TEST_DIR} AND IS_DIRECTORY ${APP_TEST_TEST_DIR})
        message(STATUS "Setting up tests for ${APP_NAME} in ${APP_TEST_TEST_DIR}")

        # Create test output directory
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/apps/${APP_NAME}/tests)

        # Define variables to be used in the test directory
        # IMPORTANT: Use CACHE variables to ensure they're visible in subdirectories
        set(APP_TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/apps/${APP_NAME}/tests" CACHE INTERNAL "")
        set(APP_TEST_NAME "${APP_NAME}" CACHE INTERNAL "")

        # Add the test directory
        add_subdirectory(${APP_TEST_TEST_DIR})

        # Clear cache variables after use
        unset(APP_TEST_OUTPUT_DIR CACHE)
        unset(APP_TEST_NAME CACHE)
    else()
        message(STATUS "No tests found for ${APP_NAME}")
    endif()
endfunction()

# Template application CMake configuration

# Set minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Define the template app project
set(APP_NAME VEMU)

add_library(RV32I
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RV32I_registers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RV32I_memory.c

)

target_use_external(RV32I LIBRARIES vtuzkicc)

target_include_directories(RV32I
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)
# Add include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Define source files
set(APP_SOURCES
    main.c
)

# Create executable target
add_executable(${APP_NAME} ${APP_SOURCES})
target_link_libraries(VEMU
    PUBLIC
        RV32I
        vtuzkicc
)
target_include_directories(VEMU
    PUBLIC
        "${PROJECT_BINARY_DIR}"
)

# Link against external libraries
# Replace vtuzkicc with the libraries your application actually needs
target_use_external(${APP_NAME} LIBRARIES vtuzkicc)

# Install target
install(TARGETS ${APP_NAME}
    RUNTIME DESTINATION bin
)

# Add tests if testing is enabled
if(RE_LEARN_BUILD_TESTS)
    add_subdirectory(tests)
endif()

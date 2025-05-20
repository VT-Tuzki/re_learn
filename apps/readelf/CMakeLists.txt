# Template application CMake configuration

# Set minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Define the template app project
set(APP_NAME read_elf)

# Add include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Define source files
set(APP_SOURCES
    src/read_elf.c
    main.c
)

# Create executable target
add_executable(${APP_NAME} ${APP_SOURCES})

# Link against external libraries
# Replace vtuzkicc with the libraries your application actually needs
target_use_external(${APP_NAME} LIBRARIES cmockery)

# Install target
install(TARGETS ${APP_NAME}
    RUNTIME DESTINATION bin
)

# Add tests if testing is enabled
if(RE_LEARN_BUILD_TESTS)
    add_subdirectory(tests)
endif()

# CMockery CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(cmockery C)

# Set source files
set(CMOCKERY_SOURCES
    src/cmockery.c
)

# Set header files
set(CMOCKERY_HEADERS
    src/google/cmockery.h
)

# Create static library
add_library(cmockery STATIC ${CMOCKERY_SOURCES} ${CMOCKERY_HEADERS})

# Set include directories - point to the directory containing cmockery.h
target_include_directories(cmockery
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/google>
        $<INSTALL_INTERFACE:include>
)
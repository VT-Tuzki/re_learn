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

add_library(cmockery_compiler_flags INTERFACE)
target_compile_features(cmockery_compiler_flags INTERFACE c_std_11)
set(gcc_like_c "$<COMPILE_LANG_AND_ID:C,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_c "$<COMPILE_LANG_AND_ID:C,MSVC>")
target_compile_options(cmockery_compiler_flags INTERFACE
  "$<${gcc_like_c}:$<BUILD_INTERFACE:-Wall;-Wno-pointer-to-int-cast;-Wno-int-to-pointer-cast>>"
  "$<${msvc_c}:$<BUILD_INTERFACE:-W3>>"
)


# Create static library
add_library(cmockery STATIC ${CMOCKERY_SOURCES} ${CMOCKERY_HEADERS})
target_link_libraries(cmockery PRIVATE cmockery_compiler_flags)

# Set include directories - point to the directory containing cmockery.h
target_include_directories(cmockery
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/google>
        $<INSTALL_INTERFACE:include>
)
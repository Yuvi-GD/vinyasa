cmake_minimum_required(VERSION 3.20)
project(vinyasa LANGUAGES C)

option(VINYASA_BUILD_SANDBOX "Build the Vinyasa sandbox" OFF)
option(VINYASA_BUILD_TESTS "Build the Vinyasa tests" OFF)

# Use a compiled library once the core has implementation sources.
add_library(vinyasa INTERFACE)
add_library(vinyasa::vinyasa ALIAS vinyasa)
target_include_directories(vinyasa INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
target_compile_features(vinyasa INTERFACE c_std_11)

if(VINYASA_BUILD_SANDBOX)
    add_subdirectory(examples/sandbox)
endif()

if(VINYASA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

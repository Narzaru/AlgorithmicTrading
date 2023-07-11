cmake_minimum_required(VERSION 3.22.1)

project(function_model CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        tabulated_function.cc)

set(HEADER_FILES
        tabulated_function.h
        forward_declarations.h)

add_subdirectory(../../libraries/math math_analysis_binary)

add_library(function_model ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(function_model PRIVATE ../../libraries/)
target_link_libraries(function_model math_analysis)

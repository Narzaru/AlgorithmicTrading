cmake_minimum_required(VERSION 3.22.1)

project(qt_models CXX)

add_subdirectory(function)

file(GLOB INCLUDE "include/*")

add_library(qt_models INTERFACE ${INCLUDE})
target_link_libraries(qt_models INTERFACE function_model)
target_include_directories(qt_models INTERFACE .)

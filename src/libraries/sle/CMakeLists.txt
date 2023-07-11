cmake_minimum_required(VERSION 3.22.1)

project(sle)

set(CMAKE_CXX_STANDARD 17)

add_compile_options()

add_library(sle STATIC matrix.h matrix.cc
        sle.h sle.cc)

add_library(solvers STATIC abstract_solver.h abstract_solver.cc
        solver.h solver.cc
        parallel_solver.h
        parallel_solver.cc)

target_link_libraries(solvers sle)

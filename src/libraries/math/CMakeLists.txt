cmake_minimum_required(VERSION 3.22.1)

project(math_analysis CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        interpolation/spline_interpolation.cc
        approximation/spline_approximation.cc
        utils/interval.cc)

set(HEADER_FILES
        interpolation/spline_interpolation.h
        approximation/spline_approximation.h
        utils/interval.h
        utils/point.h
        utils/points.h)

add_subdirectory(../csv csv_binary)
add_subdirectory(../sle sle_binary)

add_library(math_analysis ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(math_analysis PUBLIC . ../)
target_link_libraries(math_analysis csv sle solvers)

# Gtest
file(GLOB TEST_SOURCE_FILES "tests/*")

find_package(GTest REQUIRED)

add_executable(math_analysis_tests ${TEST_SOURCE_FILES})

target_include_directories(math_analysis_tests PRIVATE algorithms)

target_link_libraries(math_analysis_tests
        math_analysis
        GTest::gtest
        GTest::gtest_main)

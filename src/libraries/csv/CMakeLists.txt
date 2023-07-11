cmake_minimum_required(VERSION 3.22.1)

SET(CMAKE_CXX_STANDARD 17)

project(csv)

# library files
set(SOURCE_FILES file.cc row.cc)
set(SOURCE_HEADERS file.h reader.h row.h utils.h)

add_library(csv)
target_sources(csv
        PRIVATE ${SOURCE_FILES} ${SOURCE_HEADERS})
target_include_directories(csv
        PUBLIC ../..)
target_compile_options(csv PUBLIC)

# Gtest
find_package(GTest REQUIRED)

file(COPY tests/dataset DESTINATION .)
file(GLOB TEST_SOURCE_FILES tests/*.cc)

add_executable(csv_tests)
target_sources(csv_tests
        PRIVATE ${TEST_SOURCE_FILES})
target_include_directories(csv_tests
        PRIVATE .)
target_link_libraries(csv_tests
        PRIVATE csv GTest::gtest GTest::gtest_main)

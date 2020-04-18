cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(aligned CXX)

option(ALIGNED_ENABLE_TESTS "Enable tests build" ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(common)

add_library(aligned INTERFACE)

target_compile_features(aligned INTERFACE cxx_std_17)
target_include_directories(aligned INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )

if (ALIGNED_ENABLE_TESTS)
    message(STATUS "Generating tests...")

    find_package(gtest REQUIRED)

    add_executable(aligned_allocator_test test/aligned_allocator_test.cpp)
    target_compile_features(aligned_allocator_test INTERFACE cxx_std_17)
    target_link_libraries(aligned_allocator_test PRIVATE gtest gtest_main aligned)
    target_include_directories(aligned_allocator_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    add_test(NAME aligned_allocator_test COMMAND aligned_allocator_test)

    add_executable(aligned_vector_test test/aligned_vector_test.cpp)
    target_compile_features(aligned_vector_test INTERFACE cxx_std_17)
    target_link_libraries(aligned_vector_test PRIVATE gtest gtest_main aligned)
    target_include_directories(aligned_vector_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    add_test(NAME aligned_vector_test COMMAND aligned_allocator_test)
endif ()

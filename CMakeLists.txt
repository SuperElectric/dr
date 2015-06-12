cmake_minimum_required (VERSION 2.6)
project (dr)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake_modules/)
message(STATUS "cmake_module_path: " ${CMAKE_MODULE_PATH})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(GTEST_DIR ${PROJECT_SOURCE_DIR}/external_libraries/gtest-1.7.0)
set(GTEST_BOTH_LIBRARIES gtest gtest_main)

add_subdirectory (external_libraries/gtest-1.7.0)
add_subdirectory (src)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

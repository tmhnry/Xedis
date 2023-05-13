cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(avl_test VERSION 1.0)

add_executable(avl_test avl_test.cpp avl_tree.cpp)
target_include_directories(avl_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

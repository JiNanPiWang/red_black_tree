cmake_minimum_required(VERSION 3.27)
project(red_black_tree)

set(CMAKE_CXX_STANDARD 14)

add_executable(red_black_tree rbtree-tst.c rbtree.h rbtree.c)

cmake_minimum_required(VERSION 3.23)
project(tuple_test C)

set(CMAKE_C_STANDARD 99)

add_executable(tuple_test main.c tuple.h main.h tuple.c)

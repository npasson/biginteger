cmake_minimum_required(VERSION 3.12)
project(BigInteger)

set(CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

add_executable(BigInteger main.cpp _biginteger_t.h)
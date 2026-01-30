cmake_minimum_required(VERSION 3.13)

project(prime)

add_subdirectory(multiprecision)
add_executable(prime prime.cpp)

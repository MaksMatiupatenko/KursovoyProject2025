cmake_minimum_required(VERSION 3.13)

project(prime)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED)

add_executable(prime main.cpp Random.cpp FirstPrimes.cpp)
target_link_libraries(prime PRIVATE Boost::headers) 

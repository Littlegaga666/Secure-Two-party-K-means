cmake_minimum_required(VERSION 3.12)
project(libpaillier LANGUAGES CXX)

find_package(GMP QUIET)
add_library(libpaillier paillier.cpp)
add_library(BIOSECURE::paillier ALIAS libpaillier)
target_link_libraries(libpaillier PUBLIC GMP::GMP)



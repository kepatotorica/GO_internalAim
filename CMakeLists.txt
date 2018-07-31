cmake_minimum_required(VERSION 3.10)
project(GO_internalAim)

set(CMAKE_CXX_STANDARD 17)

add_library(GO_internalAim SHARED library.cpp library.h)
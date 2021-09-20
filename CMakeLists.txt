cmake_minimum_required(VERSION 3.20)
project(oscprotocool)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

add_library(OSCprotoCool OSCC_core.cpp OSCC_core.hpp arrayConsumer.cpp arrayConsumer.hpp OSCC_read.cpp OSCC_read.hpp OSCC_types.cpp OSCC_types.hpp)
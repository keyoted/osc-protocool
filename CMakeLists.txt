cmake_minimum_required(VERSION 3.20)
project(OSCprotoCool VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

add_library(
        OSCC STATIC
        oscc_read.cpp
        oscc_core.cpp
        oscc_write.cpp
        oscc_types_bundle.cpp
        oscc_types_message.cpp
        oscc_util.cpp
)

add_executable(
        executable
        main.cpp
)

target_link_libraries(
        executable PRIVATE
        OSCC
)

cmake_minimum_required(VERSION 3.20)
project(OSCprotoCool VERSION 0.0.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

add_compile_definitions(OSCC_TYPE_NON_STD)
add_library(
        OSCC STATIC
        lib/oscc_read.cpp
        lib/oscc_write.cpp
        lib/oscc_types_bundle.cpp
        lib/oscc_types_message.cpp
        lib/oscc_types_packet.cpp
        lib/oscc_address_space.cpp
        lib/oscc_util.cpp
        lib/oscc_util_string_conversions.cpp
        lib/oscc_util_byte_conversions.cpp)

add_executable(
        executable
        main.cpp
)

target_link_libraries(
        executable PRIVATE
        OSCC
)

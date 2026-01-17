cmake_minimum_required(VERSION 4.1)
project(Dash
        VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s -flto")

add_executable(Dash)

add_subdirectory(src)

target_link_libraries(Dash xcb)
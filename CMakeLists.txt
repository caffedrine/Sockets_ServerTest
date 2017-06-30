cmake_minimum_required(VERSION 3.6)
project(Sockets_ServerTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++")

set(SOURCE_FILES main.cpp)
add_executable(Sockets_ServerTest ${SOURCE_FILES})
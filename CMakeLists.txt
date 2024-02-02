cmake_minimum_required(VERSION 3.16)
project(network)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE sources_msgs proto/*.cc)

add_executable(network main.cpp ${sources}
        main.cpp
)
target_include_directories(network PUBLIC include/)

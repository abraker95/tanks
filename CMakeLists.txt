cmake_minimum_required(VERSION 2.8)
project(tank)

include_directories(include)

set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -std=c++11 $(CMAKE_CXX_FLAGS)")

file(GLOB all_cpp_and_h "*.h" "*.cpp")

add_executable(tank ${all_cpp_and_h})
target_link_libraries(tank sfml-graphics sfml-window sfml-system)

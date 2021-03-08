cmake_minimum_required(VERSION 3.16.3)
project(pistache_cpp_example)

set(CMAKE_CXX_STANDARD 14)
find_package(Pistache 0.0.2 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} pistache_shared)

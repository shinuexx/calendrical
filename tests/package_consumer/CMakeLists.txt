cmake_minimum_required(VERSION 3.20)
project(calendrical_package_consumer LANGUAGES CXX)

find_package(calendrical 0.1 CONFIG REQUIRED)

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE calendrical::calendrical)

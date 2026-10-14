cmake_minimum_required(VERSION 3.20)
project(calendrical_package_consumer LANGUAGES CXX)

find_package(calendrical 1.0 CONFIG REQUIRED)

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE calendrical::calendrical)

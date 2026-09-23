cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_COMPILER g++-16)

project(ReflectGPU CXX)

add_executable(reflect_gpu main.cpp)
target_include_directories(reflect_gpu PRIVATE include)
target_compile_options(reflect_gpu PRIVATE -std=c++26 -freflection -fdiagnostics-color=always)

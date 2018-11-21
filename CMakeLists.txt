cmake_minimum_required(VERSION 3.2)
add_executable(nanoxel src/main.c)
add_executable(nxcalc src/calc.c)

project(nanoxel)

set(CMAKE_C_FLAGS "-nostdlib -static -masm=intel")

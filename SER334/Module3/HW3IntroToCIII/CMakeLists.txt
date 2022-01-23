cmake_minimum_required(VERSION 3.21)
project(HW3IntroToCIII C)

set(CMAKE_C_STANDARD 99)

add_executable(RodriguezRodriguezImageProcessor RodriguezRodriguezImageProcessor.c Image.c BMPHandler.c)
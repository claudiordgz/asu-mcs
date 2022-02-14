cmake_minimum_required(VERSION 3.21)
project(Notes2 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(Q8Q1 Quizz8Q1.c)
add_executable(Q8Q4 Quizz8Q4.c)

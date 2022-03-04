cmake_minimum_required(VERSION 3.21)
project(ADJ2 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(${PROJECT_NAME} 
  RequestProcessor.c
  RequestProcessor.h
  queue.h
  queue.c
  main.c
)


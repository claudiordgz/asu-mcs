cmake_minimum_required(VERSION 3.21)
project(Simulator C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(${PROJECT_NAME}
        DataLoader.h
        DataLoader.c
        PageTable.h
        PageTable.c
        Simulator.c
        )

target_link_libraries(${PROJECT_NAME} m)
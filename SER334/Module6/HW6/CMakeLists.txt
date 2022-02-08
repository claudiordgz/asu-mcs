cmake_minimum_required(VERSION 3.21)
project(RodriguezRodriguezFilters C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(${PROJECT_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/BmpProcessor.o
    RodriguezRodriguezFilters.c
    FilterProcessor.c
    BmpProcessor.h
    PixelProcessor.h
    FilterProcessor.h
)
target_link_libraries(${PROJECT_NAME} m)
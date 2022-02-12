cmake_minimum_required(VERSION 3.21)
project(RodriguezRodriguezLoadBalancer C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(${PROJECT_NAME} 
    InstanceHost.h
    RodriguezRodriguezInstanceHost.c
    LoadBalancer.h
    RodriguezRodriguezLoadBalancer.c
    User.c
)
target_link_libraries(${PROJECT_NAME} m)
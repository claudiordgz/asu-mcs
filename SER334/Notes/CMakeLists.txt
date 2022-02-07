cmake_minimum_required(VERSION 3.21)
project(Notes C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_library(local-lib SHARED Module3/LinkedList.c)

add_executable(Module2P1 Module2/1MemoryAllocation.c)
add_executable(Module2P2 Module2/2MemoryAllocation.c)
add_executable(Module2P3 Module2/3DefiningNewTypes.c)
add_executable(Module2P4 Module2/4DefiningNewTypes.c)
add_executable(Module2P5 Module2/5ParametersScopeAndPointers.c)
add_executable(Module2P6 Module2/6ParametersScopeAndPointers.c)
add_executable(Module2P7 Module2/7LinkedLists.c)
add_executable(Module2P8 Module2/8LinkedLists.c)
add_executable(Module2NStruct1 Module2/99NoteStruct1.c)
add_executable(Module2QP2 Module2/98Quiz2.c)
add_executable(Module2QP6 Module2/98Quiz6.c)

add_executable(Module3QP3 Module3/98Quiz3.c)
add_executable(Module3QP4 Module3/98Quiz4.c)
add_executable(Module3QP5 Module3/98Quiz5.c Module3/LinkedList.h Module3/LinkedList.c)

add_executable(Module6NoteForks Module6/NoteForks.c)
add_executable(Module6Q6Q6 Module6/Q6Q6.c)

add_executable(Module7Q7Q7 Module7/Q7Q7.c)


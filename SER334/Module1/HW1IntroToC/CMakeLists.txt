cmake_minimum_required(VERSION 3.21)
project(HW1IntroToC C)

set(CMAKE_C_STANDARD 99)

add_executable(Computation RodriguezRodriguezComputation.c)
add_executable(Collatz RodriguezRodriguezCollatz.c)
add_executable(Decryption RodriguezRodriguezDecrypt.c)
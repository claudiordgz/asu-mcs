cmake_minimum_required(VERSION 3.21)
project(CSE340_Project2_Spring_2022)

set(CMAKE_CXX_STANDARD 14)

add_executable(CSE340_Project2_Spring_2022 parser.cc parser.h lexer.cc lexer.h inputbuf.cc inputbuf.h)

cmake_minimum_required(VERSION 3.25)
project(RSA_Cryptography C)

set(CMAKE_C_STANDARD 17)

add_executable(RSA_Cryptography main.c optimized_main.c)

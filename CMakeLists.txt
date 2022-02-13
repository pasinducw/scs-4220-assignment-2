cmake_minimum_required(VERSION 3.19)
project(assignment2)

set(CMAKE_CXX_STANDARD 11)
add_executable(assignment2
        main.cpp
        lib/VEBTree.cpp lib/VEBTree.h
        lib/FibonacciHeap.cpp lib/FibonacciHeap.h
        )
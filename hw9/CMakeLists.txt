cmake_minimum_required(VERSION 4.4.1)
project(multithread_count_words CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(test 
    topk_words.cpp
)

add_executable(mt1
    mt1.cpp
)

add_executable(mt2
    mt2.cpp
)
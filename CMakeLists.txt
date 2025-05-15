cmake_minimum_required(VERSION 3.28)
project(Matam_HW1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Matam_HW1
        Transaction.cpp
        Utilities.cpp
        BlockChain.cpp
        main.cpp
)
cmake_minimum_required(VERSION 3.10)
project(eBPF_example)

include_directories(/usr/include/bcc/compat/)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER clang++) # or clang++ or g++

add_executable(helloworld src/HelloWorld.cpp)
add_executable(cpudistribution src/CPUDistribution.cpp)
add_executable(blockcharhist src/BlockCharHist.cpp)
add_executable(looptest src/LoopTest.cpp)
add_executable(tailcall src/TailCall.cpp)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -unroll-count=10 -O2 -Ofast -ffast-math -funroll-loops -march=native") # get crazy here

target_link_libraries(helloworld bcc)
target_link_libraries(cpudistribution bcc)
target_link_libraries(blockcharhist bcc)
target_link_libraries(looptest bcc)
target_link_libraries(tailcall bcc)
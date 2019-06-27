cmake_minimum_required(VERSION 3.14)
project(eBPF_example)

include_directories(/usr/include/bcc/compat/)
set(CMAKE_CXX_STANDARD 11)

add_executable(helloworld src/HelloWorld.cpp)
add_executable(cpudistribution src/CPUDistribution.cpp)

target_link_libraries(helloworld bcc)
target_link_libraries(cpudistribution bcc)
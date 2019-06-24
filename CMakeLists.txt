cmake_minimum_required(VERSION 3.14)
project(eBPF_example)

include_directories(/usr/include/bcc/compat/)
set(CMAKE_CXX_STANDARD 11)

add_executable(ebpf src/HelloWorld.cpp)
target_link_libraries(ebpf bcc)
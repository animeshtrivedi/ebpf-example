#!/bin/bash

CFLAGS="-O2" 

# /tools/lib/bpf/ in the linux src code is where libbpf is build. I should be able to link it against my libbpf build from the github. 
INCLUDES="-I/home/atr/vu/github/torvalds/linux/tools/lib/ -I./from_kernel/ -I/home/atr/vu/github/torvalds/linux/tools/perf/ -I/home/atr/vu/github/torvalds/linux/tools/include/"
#          ^^ fatal error: bpf/bpf.h: No such file or directory
#                                                         ^^ local helpers copied from the linux src code (fatal error: bpf_load.h: No such file or directory)   
#                                                                          ^^ linking to the perf infra to get event triggers (fatal error: perf-sys.h: No such file or directory)
#                                                                                                                          ^^^ fatal error: linux/compiler.h: No such file or directory

#this links with the dynamic lib, which is problematic as I cannot find a reliable way to setup LD_LIB_PATH with sudo permissions 
#gcc cpustat_user.c ./from_kernel/bpf_load.c -I/home/atr/vu/github/torvalds/linux/tools/lib/ -I./from_kernel/ -I/home/atr/vu/github/torvalds/linux/tools/perf/ -I/home/atr/vu/github/torvalds/linux/tools/include/ -L/home/atr/vu/github/torvalds/linux/tools/lib/bpf/ -lbpf -lelf -o cpustat_user

#This compiles in statically (see .a)
gcc cpustat_user.c ./from_kernel/bpf_load.c /home/atr/vu/github/torvalds/linux/tools/lib/bpf/libbpf.a $CFLAGS $INCLUDES -lelf -o cpustat

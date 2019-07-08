#!/bin/bash
#set -x 
set -e 
clang -O2 -Wall -target bpf -nostdinc -I "/usr/include/" -I "/usr/include/x86_64-linux-gnu/" -c ./xdp_drop.c -o xdp_drop.o  
clang -g -O2 -Wall -target bpf -nostdinc -I "/usr/include/" -I "/usr/include/x86_64-linux-gnu/" -c ./xdp_drop.c -o xdp_drop.S  
echo "Sucess" 
echo "Additional information can be checked with (1) file xdp_drop.o; (2) readelf -a xdp_drop.o" 

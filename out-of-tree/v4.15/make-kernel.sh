#!/bin/bash
CFLAGS="-O2 -D__KERNEL__ -Wno-unused-value -Wno-pointer-sign -D__TARGET_ARCH_x86 -Wno-compare-distinct-pointer-types -Wno-gnu-variable-sized-type-not-at-end -Wno-address-of-packed-member -Wno-tautological-compare -Wno-unknown-warning-option"
INCX="-I/usr/include/ -I/usr/include/x86_64-linux-gnu/ -I/usr/src/linux-headers-`uname -r`/include/ -I./from_kernel/"
#      ^^for linux/verion.h
#                      ^^for fatal error: 'asm/types.h' file not found
#                                                      ^^ for 'uapi/linux/bpf.h' file not found 
#                                                                                                  ^^ local helper headers in the kernel 

#this works
#clang -target bpf -nostdinc -I/usr/lib/clang/6.0/include/ -I /usr/include/ -I /usr/include/x86_64-linux-gnu/ -I /usr/src/linux-headers-4.15.0-1045-oem/include/ -I ./from_kernel/ -O2 -emit-llvm -c cpustat_kern.c -o - | llc -march=bpf -filetype=obj -o cpustat_kern.o
set -x 
echo "clang -target bpf -nostdinc $INCX $CFLAGS -emit-llvm -c cpustat_kern.c -o - | llc -march=bpf -filetype=obj -o cpustat_kern.o"
clang -target bpf -nostdinc $INCX $CFLAGS -emit-llvm -c cpustat_kern.c -o - | llc -march=bpf -filetype=obj -o cpustat_kern.o
file cpustat_kern.o 



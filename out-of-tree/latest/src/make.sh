#!/bin/bash
#clang -target bpf , see https://www.kernel.org/doc/html/latest/bpf/bpf_devel_QA.html#q-clang-flag-for-target-bpf
# also : https://stackoverflow.com/questions/56975861/error-compiling-ebpf-c-code-out-of-kernel-tree

C_FLAGS="-Wno-unused-value -Wno-pointer-sign -Wno-compare-distinct-pointer-types -Wno-gnu-variable-sized-type-not-at-end -Wno-address-of-packed-member -Wno-tautological-compare -Wno-unknown-warning-option"

function magic_make {
DEFAULT=$1
KERN="$DEFAULT"_kern
USER="$DEFAULT"_user
LIBBPF_ROOT="/home/atr/src/libbpf"
LIBBPF_A="$LIBBPF_ROOT/src/libbpf.a"
LIBBPF_UAPI="$LIBBPF_ROOT/include/uapi/"
LIBBPF_SRC="$LIBBPF_ROOT/src/"

clang -nostdinc -isystem `clang -print-file-name=include` \
	-D__KERNEL__ -D__ASM_SYSREG_H -D__TARGET_ARCH_x86 \
	$C_FLAGS \
	-I../common/ \
	-include /usr/src/linux-headers-`uname -r`/include/linux/kconfig.h \
	-I/usr/src/linux-headers-`uname -r`/include/ \
	-I/usr/src/linux-headers-`uname -r`/include/uapi/ \
	-I/usr/src/linux-headers-`uname -r`/include/generated/uapi/ \
	-I/usr/src/linux-headers-`uname -r`/arch/x86/include/ \
	-I/usr/src/linux-headers-`uname -r`/arch/x86/include/uapi/ \
	-I/usr/src/linux-headers-`uname -r`/arch/x86/include/generated/ \
	-I/usr/src/linux-headers-`uname -r`/arch/x86/include/generated/uapi/ \
	-O2 -emit-llvm -c "$KERN".c -o -| llc -march=bpf -filetype=obj -o "$KERN".o

gcc "$USER".c ../common/bpf_load.c $LIBBPF_A -I$LIBBPF_UAPI -I$LIBBPF_SRC -I../common/ -lelf -o $DEFAULT

}

targets=( cpustat tracex1 hellotrace )

for t in "${targets[@]}" ; do
	echo "making ...$t"
	magic_make $t
done

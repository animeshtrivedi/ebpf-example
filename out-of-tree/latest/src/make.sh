#!/bin/bash
#clang -target bpf , see https://www.kernel.org/doc/html/latest/bpf/bpf_devel_QA.html#q-clang-flag-for-target-bpf
# also : https://stackoverflow.com/questions/56975861/error-compiling-ebpf-c-code-out-of-kernel-tree

C_FLAGS="-Wno-unused-value -Wno-pointer-sign -Wno-compare-distinct-pointer-types -Wno-gnu-variable-sized-type-not-at-end -Wno-address-of-packed-member -Wno-tautological-compare -Wno-unknown-warning-option"

function magic_make {
# You can configure these two params 
LINUX_HDRS_ROOT="/usr/src/linux-headers-`uname -r`"
LIBBPF_ROOT="/home/atr/vu/github/libbpf"

DEFAULT=$1
KERN="$DEFAULT"_kern
USER="$DEFAULT"_user
LIBBPF_A="$LIBBPF_ROOT/src/libbpf.a"
LIBBPF_UAPI="$LIBBPF_ROOT/include/uapi/"
LIBBPF_SRC="$LIBBPF_ROOT/src/"
set -x 
clang -nostdinc -isystem `clang -print-file-name=include` \
	-D__KERNEL__ -D__ASM_SYSREG_H -D__TARGET_ARCH_x86 \
	$C_FLAGS \
	-I../common/ \
	-include $LINUX_HDRS_ROOT/include/linux/kconfig.h \
	-I$LINUX_HDRS_ROOT/include/ \
	-I$LINUX_HDRS_ROOT/include/uapi/ \
	-I$LINUX_HDRS_ROOT/include/generated/uapi/ \
	-I$LINUX_HDRS_ROOT/arch/x86/include/ \
	-I$LINUX_HDRS_ROOT/arch/x86/include/uapi/ \
	-I$LINUX_HDRS_ROOT/arch/x86/include/generated/ \
	-I$LINUX_HDRS_ROOT/arch/x86/include/generated/uapi/ \
	-O2 -emit-llvm -c "$KERN".c -o -| llc -march=bpf -filetype=obj -o "$KERN".o
set +x 

gcc "$USER".c ../common/bpf_load.c $LIBBPF_A -I$LIBBPF_UAPI -I$LIBBPF_SRC -I../common/ -lelf -o $DEFAULT

}

targets=( cpustat tracex1 hellotrace )

for t in "${targets[@]}" ; do
	echo "making ...$t"
	magic_make $t
done

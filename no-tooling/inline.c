#include <linux/bpf.h>

#ifndef __section
# define __section(NAME)                  \
   __attribute__((section(NAME), used))
#endif

__section("prog")
int xdp_test(struct xdp_md *ctx)
{
    __u64 a = 2, b = 3, *c = &a;
    /* just a toy xadd example to show the syntax */
    asm volatile("lock *(u64 *)(%0+0) += %1" : "=r"(c) : "r"(b), "0"(c));
    return a;
}

char __license[] __section("license") = "GPL";

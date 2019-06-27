//
// Created by atr on 6/27/19.
//

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>

#include <bcc/BPF.h>

// somehow loop unrolling (without putting anything) until 99 work? It somehow depends what you do inside the loop
// one you put the pragma then the stack limit is exceeded
// error: <unknown>:0:0: in function on_syscall_read i32 (i8*): Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.

// R - raw string literal
const std::string BPF_PROGRAM = R"(
#include <uapi/linux/ptrace.h>
#include <net/sock.h>
#include <bcc/proto.h>

int on_syscall_read(void *ctx) {
  int i = 0;
  uint64_t sum=0;
  for(i=0;i<32;i++){
   bpf_trace_printk("i = %d \n", i);
   sum+=bpf_ktime_get_ns();
  }
  bpf_trace_printk("LoopTest: (triggered via read syscall) sum=%llu , i = %d ctx=%p \n", sum, i, ctx);
  return 0;
}
)";

int main() {
    ebpf::BPF bpf;
    auto init_res = bpf.init(BPF_PROGRAM);
    if (init_res.code() != 0) {
        std::cerr << init_res.msg() << std::endl;
        return 1;
    }

    std::ifstream pipe("/sys/kernel/debug/tracing/trace_pipe");
    std::string line;
    // you get the name of the function to attached to
    std::string clone_fnname = bpf.get_syscall_fnname("read");
    std::cout << "sys funciton name is " << clone_fnname << std::endl;
    auto attach_res = bpf.attach_kprobe(clone_fnname, "on_syscall_read");
    if (attach_res.code() != 0) {
        std::cerr << attach_res.msg() << std::endl;
        return 1;
    }

    while (true) {
        if (std::getline(pipe, line)) {
            std::cout << line << std::endl;
            // Detach the probe if we got at least one line.
            auto detach_res = bpf.detach_kprobe(clone_fnname);
            if (detach_res.code() != 0) {
                std::cerr << detach_res.msg() << std::endl;
                return 1;
            }
            break;
        } else {
            std::cout << "Waiting for a sys_clone event" << std::endl;
            sleep(1);
        }
    }

    return 0;
}

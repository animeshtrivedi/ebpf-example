//
// Created by atr on 6/27/19.
//

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>

#include <bcc/BPF.h>
#include <sstream>


const std::string BPF_PROGRAM = R"(
BPF_PROG_ARRAY(prog_array, 10);

int tail_call(void *ctx) {
    bpf_trace_printk("Tail-call\n");
    return 0;
}

int do_tail_call(void *ctx) {
    bpf_trace_printk("Original program\n");
    prog_array.call(ctx, 2);
    return 0;
}
)";

int main(int argc, char**argv) {
    ebpf::BPF bpf;
    if(argc >= 2) {
        std::cout << "Going to load the eBPF program from (please make sure that this is a valid path)" << argv[1]
                  << std::endl;
        std::ifstream t(argv[1]);
        std::stringstream buffer;
        buffer << t.rdbuf();
        const std::string file_ebpf = buffer.str();
        auto init_res = bpf.init(file_ebpf);
        if (init_res.code() != 0) {
            std::cerr << init_res.msg() << std::endl;
            return 1;
        }
    } else {
        auto init_res = bpf.init(BPF_PROGRAM);
        if (init_res.code() != 0) {
            std::cerr << init_res.msg() << std::endl;
            return 1;
        }
    }

    int fd;
    auto load_res = bpf.load_func("tail_call", BPF_PROG_TYPE_KPROBE, fd);
    if(load_res.code() != 0){
        std::cerr << load_res.msg() << std::endl;
        return 1;
    }

    ebpf::BPFProgTable prog_table = bpf.get_prog_table("prog_array");
    std::cout<<"The map capacity is " << prog_table.capacity() << std::endl;
    auto update_res = prog_table.update_value(2, fd);
    if(update_res.code() != 0){
        std::cerr << update_res.msg() << std::endl;
        return 1;
    }

    std::ifstream pipe("/sys/kernel/debug/tracing/trace_pipe");
    std::string line;
    // you get the name of the function to attached to
    std::string clone_fnname = bpf.get_syscall_fnname("read");
    std::cout << "sys function name is " << clone_fnname << std::endl;
    auto attach_res = bpf.attach_kprobe(clone_fnname, "do_tail_call");
    if (attach_res.code() != 0) {
        std::cerr << attach_res.msg() << std::endl;
        return 1;
    }

    while (true) {
        if (std::getline(pipe, line)) {
            std::cout << line << std::endl;
            std::cout << "Waiting 1 seconds for printk(s) to appear in debug/tracing/trace_pipe | but then it will continue to run the probe" << std::endl;
            sleep(1);
            // Detach the probe if we got at least one line.
            auto detach_res = bpf.detach_kprobe(clone_fnname);
            if (detach_res.code() != 0) {
                std::cerr << detach_res.msg() << std::endl;
                return 1;
            }
            break;
        } else {
            std::cout << "Waiting for a " << clone_fnname << " event" << std::endl;
            sleep(1);
        }
    }

    return 0;
}
//
// Created by atr on 6/27/19.
//

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>

#include <bcc/BPF.h>

// R - raw string literal
const std::string BPF_PROGRAM = R"(
int kprobe__tcp_v4_connect(struct pt_regs *ctx, struct sock *sk)
{
	u32 pid = bpf_get_current_pid_tgid();
	// stash the sock ptr for lookup on return
	currsock.update(&pid, &sk);
	return 0;
};
)";

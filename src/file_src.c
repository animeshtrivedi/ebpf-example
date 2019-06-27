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
## How to setup and run 

### Top-level examples
https://github.com/iovisor/bcc
 
### BCC cpp examples 
https://github.com/iovisor/bcc/tree/master/examples/cpp

### BCC FAQ 
https://github.com/iovisor/bcc/blob/master/FAQ.txt

###### Where do i see the output of bpf_trace_printk? 
`sudo cat /sys/kernel/debug/tracing/trace_pipe` and then run your ebPF program. 

Alternatively, you can also just open the pipe in the C++ code like in the [HelloWorld.cc example](https://github.com/iovisor/bcc/blob/master/examples/cpp/HelloWorld.cc).
 
You can reset the buffer : [https://github.com/iovisor/bcc/blob/master/tools/reset-trace_example.txt](https://github.com/iovisor/bcc/blob/master/tools/reset-trace_example.txt)

###### What is the syntax of ebpf_printk message

```
In your example:

telnet-470   [001] .N.. 419421.045894: 0x00000001: BPF command: 2
telnet is your current task's name.
470 is your current task's PID.
001 is the CPU number on which the task is running.
In .N.., each character refers to a set of options (whether irqs are enabled, scheduling options, whether hard/softirqs are running, level of preempt_disabled respectively). N means that TIF_NEED_RESCHED and PREEMPT_NEED_RESCHED are set.
419421.045894 is a timestamp.
0x00000001 is a fake value used by BPF for the ip register.
BPF command: 2 is your message.
```

[https://stackoverflow.com/questions/47914219/output-from-bpf-printk](https://stackoverflow.com/questions/47914219/output-from-bpf-printk)

     1	# tracer: nop
     2	#
     3	#                              _-----=> irqs-off
     4	#                             / _----=> need-resched
     5	#                            | / _---=> hardirq/softirq
     6	#                            || / _--=> preempt-depth
     7	#                            ||| /     delay
     8	#           TASK-PID   CPU#  ||||    TIMESTAMP  FUNCTION
     9	#              | |       |   ||||       |         |

**Note:** There is no reliability in getting to see your printk message. Apprently, it depends if the message got 
written before your eBPF program is finsihed then nothing shows. This happens frequently with the structure of 
the code I am writing as I am waiting for the first line to pop up, print that, and remove the hook.  
###### Loop unrolling


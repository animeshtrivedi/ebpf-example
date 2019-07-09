set -e 
clang -O2 -Wall -target bpf -nostdinc -I "/usr/include/" -I "/usr/include/x86_64-linux-gnu/" -c inline.c -o inline.o
clang -g -O2 -Wall -target bpf -nostdinc -I "/usr/include/" -I "/usr/include/x86_64-linux-gnu/" -c inline.c -o inline.S 

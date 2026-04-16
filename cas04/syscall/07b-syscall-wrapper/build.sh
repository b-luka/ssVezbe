as -g -o syscall_wrapper.o syscall_wrapper.s

ld -o executable \
	--dynamic-linker /lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 \
	syscall_wrapper.o \
	--library c

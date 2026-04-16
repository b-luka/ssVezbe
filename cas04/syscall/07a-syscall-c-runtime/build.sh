# gcc -g -o executable syscall_c_runtime.s
# objdump --full-contents --section=.interp executable
# ldd executable

as -g -o syscall_c_runtime.o syscall_c_runtime.s

ld -o executable \
	--dynamic-linker /lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 \
	syscall_c_runtime.o \
	/lib/x86_64-linux-gnu/crt1.o \
	/lib/x86_64-linux-gnu/crti.o \
	/lib/x86_64-linux-gnu/crtn.o \
	--library c \
	

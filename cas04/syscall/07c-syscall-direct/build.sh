as -g -o syscall_direct.o syscall_direct.s

ld --entry=custom_entry -o executable syscall_direct.o

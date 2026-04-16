.intel_syntax noprefix

.global custom_entry

.equ message_len, message_end - message_start

.data
message_start:
.asciz "Hello World!\n"
message_end:

.text       # nema ni libc, nema posrednika za syscall - pisemo ih mi
custom_entry:
    # man 2 intro
    # man 2 syscall
    # man 2 syscalls
    
    # echo  SYS_write | cpp --include sys/syscall.h -
    mov rax, 1                    # write
    mov rdi, 1                    # fd
    mov rsi, offset message_start # buf
    mov rdx, offset message_len   # count
    syscall

    # echo  SYS_exit | cpp --include sys/syscall.h -
    mov rax, 60                   # exit
    mov rdi, 13                   # status
    syscall
.end

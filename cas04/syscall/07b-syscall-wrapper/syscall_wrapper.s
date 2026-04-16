.intel_syntax noprefix

.global _start

.extern write
.extern _exit

.equ message_len, message_end - message_start

.data
message_start:
.ascii "Hello World!\n"
message_end:

.text
_start:     # nema c runtime biblioteka, nema ni main, ali ima libc
    # man 2 write
    mov rdi, 1                    # fd
    mov rsi, offset message_start # buf
    mov rdx, offset message_len   # count
    call write

    # man 2 exit
    mov rdi, 13                   # status
    call _exit
.end

.intel_syntax noprefix

.global main

.extern write
.extern _exit

.equ message_len, message_end - message_start
# message end se vezuje na bajt posle stringa
# .equ - 2. nacin pravljenja simbola, 1. arg ime, 2. arg izraz

.data
message_start:
.ascii "Hello World!\n"     # nema \0
message_end:

.text
main:
    # man 2 write
    # ssize_t write(int fd, const void buf[.count], size_t count);
    mov rdi, 1                    # fd
    mov rsi, offset message_start # buf
    mov rdx, offset message_len   # count
    call write
    
    # man 2 exit
    mov rdi, 8                    # status
    call _exit
.end

.intel_syntax noprefix

.global main

.extern sum
.extern printf

.data
labela_poruke:
.asciz "Sum of 1 and 3 is %d\n"

.text
main:
    push rbp
    mov rbp, rsp

    mov edi, 1
    mov esi, 3
    call sum

    mov rdi, offset labela_poruke
    mov esi, eax
    call printf

    leave
    ret

.end

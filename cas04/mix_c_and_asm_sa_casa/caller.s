.intel_syntax noprefix  # intel sintaksa

.global main    # definisanje simbola main
.extern sum     # linker ce sam razresiti i ako se ne navede ali je dobra praksa
.extern printf

.data
template:
.asciz "Sum of 5 and 5 is %d\n"       # daje ascii vrednosti, z - ima \0

.text
main:
    push rbp            # prolog funkcije
    mov rbp, rsp

    # poziv sum
    mov edi, 5          # jer je int 32b pa staje u edi
    mov esi, 5

    call sum            # rezultat u eax

    # poziv printf
    # mov rdi, template   # pokazivac je 64b, memorijski direktno adresiranje i cita samo prvih 8B
    mov rdi, offset template    # sada se cita ceo
    mov esi, eax
    call printf         # printf("template", ...);

    leave               # cisti stek
    ret                 # epilog funkcije

.end        # gcc -o exe caller.s callee.c -no-pie

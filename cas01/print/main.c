#include <stdio.h>

extern void print();

int main(int argc, char** argv) {
    //printf("Hello World!");
    print();

    // gcc -o exec main.c lib.c --save-temps
    // cuva objektne i pretprocesorske fajlove

    // gcc -masm=intel --save-temps -o exec main.c lib.c
    // masm = intel => intel sintaksa (bez procenata)
    
    // objdump -D -Mintel exec-main.o > main.objdump
    // disasemblovanje
}
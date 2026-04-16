// asm kod na moodleu
// potrebno:
// deklaracija potencijalno potrebnih tipova podataka
// deklaraciju funkcije bar i definiciju funkcije foo
// ukljuciti -fno-stack-protector i --save-temps
// nije napisano ali je koriscena optimizacija O1

// gcc -fno-stack-protector --save-temps -O1
// povratna vrednost u eax - 32b - verovatno int, moguce short ili char
// DWORD PTR - 32b i uzimamo ih sa neke adrese - int/short/char
// u rdi je rezultat funkcije bar koji je veliki jer je stek pripremljen pre poziva
// zauzeto 72B ali se poravnava na 16 tako da je moguce i 64B + padding

// posle bar se upisuju 2 DWORDa u eax na pomerajima 28 i 44 - intovi u strukturi, pretpostavimo da su svi pomeraji intovi

typedef struct {
    int array[16];     // 16 ili 18 (64B ili 72B)
} StructType1;

extern StructType1 bar(int a1, int a2, int a3, int a4);

int foo(StructType1 arg1) {
    StructType1 local = bar(2, arg1.array[4], 9, arg1.array[13]);  // 2 i 9 kao konstante u asm, + citanje sa pomeraja 96 i 132 -
    // argumenti koje prima foo koji su MEMORY klasa, 96 - 72 - 8 (ret addr) = 16, 132 - 80 = 56 - offseti za intove
    // mozemo iskoristiti StructType1 posto je minimum 16B, preracunamo polja kojima pristupamo
    return local.array[11] + local.array[7];   // offseti 44 i 28
}

// gcc test.c --save-temps -fno-stack-protector -O1 -masm=intel

// nije 18B (kad se prevede dobije se 88 umesto 72)

// ako promenimo argumente za bar na short ili char dobice se isti asm kod


// teorija
/*
1. pitanje (stek i poziv funckije)
stack frame iz dokumentacije
crvena zona se ne koristi za lokalne promenljive, ne postoji implicitna alokacija steka
odgovor je ne

2. pitanje (prosledjivanje parametara)
StructType pripada klasi MEMORY - vise od 16B i nije SSE
pozivalac alocira prostor za povratnu vrednost foo
parametri
char param0 - rsi (rdi zauzet)
double param1 - sse reg, xmm0
StructType param2 - stack push 2
void* param3 - rdx
StructType param4 - stack push 1
float param5 - xmm1
long param6 - rcx

(symbols)
niz intova se drugacije ponasa od pokazivaca na int
int c[], int *b;
c i &c imaju istu vrednost - adresu prvog elementa (array to pointer decay)
*/
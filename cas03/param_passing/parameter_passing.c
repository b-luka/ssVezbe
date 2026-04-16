#include <stdio.h>

typedef struct
{
    char f1;   // size:  1, alignment: 1
    int f2;    // size:  4, alignment: 4
    short f3;  // size:  2, alignment: 2
} StructType1; // size: 12, alignment: 4

/*
 * |        |        |        |        |        |        |        |        |
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |        |        |        |        |  padd  |  padd  |   f3   |   f3   | : 8byte[1]
 * |   f2   |   f2   |   f2   |   f2   |  padd  |  padd  |  padd  |   f1   | : 8byte[0]
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |        |        |        |        |        |        |        |        |
 *
 * gcc classify_argument()
 * https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/i386.cc#L2074
 *
 * System V ABI - 3.2.3 Parameter Passing - Klasifikacija agregatnog argumenta:
 * 1) nije ispunjeno (sizeof(StructType1) = 12)
 * 2) nije primenjivo
 * 3) svaki 8byte se klasifikuje zasebno
 * 4) svako polje se klasifikuje zasebno odnosno rekurzivno (primenom istog ovog algoritma na dato polje)
 *      8byte[0] : INTEGER <--- NO_CLASS(init), INTEGER(f1), INTEGER(f2)
 *      8byte[1] : INTEGER <--- NO_CLASS(init), INTEGER(f3)
 * 5) post merger cleanup:
 *      a) nije ispunjeno
 *      b) nije ispunjeno
 *      c) nije ispunjeno
 *      d) nije ispunjeno
 *         ---> pojedinacni 8byte objekta se prosledjuju prema klasama odredjenim u koraku 4
 *
 */

void foo_1_system_v_abi(
    char rdi,
    int rsi,
    StructType1 rdx_rcx,
    void *r8,
    StructType1 stack1PushI,
    long r9)
{
    /*
     * Prema System V ABI konvenciji (podrazumevana konvencija na linux sistemima)
     * u skladu sa kojom se vrsi prevodjenje posmatrane funkcije vazi sledece:
     *
     * Inicijalno slobodni GP (general-purpose) registri su: rdi, rsi, rdx, rcx, r8, r9
     * Inicijalno slobodni SSE (streaming simd extension) registri su: xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7
     *
     * Prvi parametar (char rdi) pripada klasi INTEGER
     *     ---> parametar : INTEGER kroz prvi naredni slobodni GP registar (rdi) i alocira citav registar
     *
     * Drugi parametar (int rsi) pripada klasi INTEGER
     *     ---> parametar : INTEGER kroz prvi naredni slobodni GP registar (rsi) i alocira citav registar
     *
     * Treci parametar (StructType1 rdx_rcx) prosledjuje svoje 8byte na sledeci nacin
     *     ---> 8byte[0] : INTEGER kroz prvi naredni slobodni GP registar (rdx) i alocira citav registar
     *     ---> 8byte[1] : INTEGER kroz prvi naredni slobodni GP registar (rcx) i alocira citav registar
     *
     * Cetvrti parametar (void *r8) pripada klasi INTEGER
     *     ---> parametar : INTEGER kroz prvi naredni slobodni GP registar (r8) i alocira citav registar
     *
     * Peti parametar (StructType1 stack1PushI) prosledjuje svoje 8byte na sledeci nacin
     *     ---> 8byte[0] : INTEGER kroz prvi naredni slobodni GP registar (r9) i alocira citav registar
     *     ---> 8byte[1] : INTEGER kroz prvi naredni slobodni GP registar (NE POSTOJI VISE SLOBODNIH GP REGISTARA)
     *     ---> sve prethodne alokacije registara se ponistavaju jer parametar ide u potpunosti preko steka
     *     ---> 8byte[0] : INTEGER preko steka na adresi (((unsigned char*) &stack1PushI) + 0 * 8)
     *     ---> 8byte[1] : INTEGER preko steka na adresi (((unsigned char*) &stack1PushI) + 1 * 8)
     *
     * Sesti parametar (long r9) pripada klasi INTEGER
     *     ---> parametar : INTEGER kroz prvi naredni slobodni GP registar (r9) i alocira citav registar
     *
     */
}

typedef struct
{
    int f1;         // size:  4, alignment: 4
    int f2;         // size:  4, alignment: 4
    short f3;       // size:  2, alignment: 2
    StructType1 f4; // size: 12, alignment: 4
    char f5;        // size:  1, alignment: 1
    short f6;       // size:  2, alignment: 2
} StructType2;      // size: 28, alignment: 4

/*
 * |        |        |        |        |        |        |        |        |
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |        |        |        |        |   f6   |   f6   |  padd  |   f5   | : 8byte[3]
 * |  padd  |  padd  | f4.f3  | f4.f3  | f4.f2  | f4.f2  | f4.f2  | f4.f2  | : 8byte[2]
 * |  padd  |  padd  |  padd  | f4.f1  |  padd  |  padd  |   f3   |   f3   | : 8byte[1]
 * |   f2   |   f2   |   f2   |   f2   |   f1   |   f1   |   f1   |   f1   | : 8byte[0]
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |        |        |        |        |        |        |        |        |
 *
 * gcc classify_argument()
 * https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/i386.cc#L2074
 *
 * System V ABI - 3.2.3 Parameter Passing - Klasifikacija agregatnog argumenta:
 * 1) nije ispunjeno (sizeof(StructType2) = 28)
 * 2) nije primenjivo
 * 3) svaki 8byte se klasifikuje zasebno
 * 4) svako polje se klasifikuje zasebno odnosno rekurzivno (primenom istog ovog algoritma na dato polje)
 *      8byte[0] : INTEGER <--- NO_CLASS(init), INTEGER(f1), INTEGER(f2)
 *      8byte[1] : INTEGER <--- NO_CLASS(init), INTEGER(f3), INTEGER(f4.f1)
 *      8byte[2] : INTEGER <--- NO_CLASS(init), INTEGER(f4.f2), INTEGER(f4.f3)
 *      8byte[3] : INTEGER <--- NO_CLASS(init), INTEGER(f5), INTEGER(f6)
 * 5) post merger cleanup:
 *      a) nije ispunjeno
 *      b) nije ispunjeno
 *      c) jeste ispunjeno ---> citav objekat pripada klasi MEMORY
 *      d) nije primenjivo
 *         ---> pojedinacni 8byte objekta se prosledjuju kao da svaki 8byte pripada klasi MEMORY
 *
 */

void foo_2_system_v_abi(
    StructType2 stack1PushI)
{
    /*
     * Prema System V ABI konvenciji (podrazumevana konvencija na linux sistemima)
     * u skladu sa kojom se vrsi prevodjenje posmatrane funkcije vazi sledece:
     *
     * Inicijalno slobodni GP (general-purpose) registri su: rdi, rsi, rdx, rcx, r8, r9
     * Inicijalno slobodni SSE (streaming simd extension) registri su: xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7
     *
     * Prvi parametar (StructType2 stack1PushI) prosledjuje svoje 8byte na sledeci nacin
     *     ---> 8byte[0] : MEMORY preko steka na adresi (((unsigned char*) &stack1PushI) + 0 * 8)
     *     ---> 8byte[1] : MEMORY preko steka na adresi (((unsigned char*) &stack1PushI) + 1 * 8)
     *     ---> 8byte[2] : MEMORY preko steka na adresi (((unsigned char*) &stack1PushI) + 2 * 8)
     *     ---> 8byte[3] : MEMORY preko steka na adresi (((unsigned char*) &stack1PushI) + 3 * 8)
     *
     */
}

typedef struct
{
    char f1;   // size:  1, alignment: 1
    char f2;   // size:  1, alignment: 1
    long f3;   // size:  8, alignment: 8
    short f4;  // size:  2, alignment: 2
    char f5;   // size:  1, alignment: 1
    int f6;    // size:  4, alignment: 4
    char f7;   // size:  1, alignment: 1
} StructType3; // size: 32, alignment: 8

/*
 * |        |        |        |        |        |        |        |        |
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |  padd  |  padd  |  padd  |  padd  |  padd  |  padd  |  padd  |   f7   | : 8byte[3]
 * |   f6   |   f6   |   f6   |   f6   |  padd  |   f5   |   f4   |   f4   | : 8byte[2]
 * |   f3   |   f3   |   f3   |   f3   |   f3   |   f3   |   f3   |   f3   | : 8byte[1]
 * |  padd  |  padd  |  padd  |  padd  |  padd  |  padd  |   f2   |   f1   | : 8byte[0]
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |        |        |        |        |        |        |        |        |
 *
 * gcc classify_argument()
 * https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/i386.cc#L2074
 *
 * System V ABI - 3.2.3 Parameter Passing - Klasifikacija agregatnog argumenta:
 * 1) nije ispunjeno (sizeof(StructType3) = 32)
 * 2) nije primenjivo
 * 3) svaki 8byte se klasifikuje zasebno
 * 4) svako polje se klasifikuje zasebno odnosno rekurzivno (primenom istog ovog algoritma na dato polje)
 *      8byte[0] : INTEGER <--- NO_CLASS(init), INTEGER(f1), INTEGER(f2)
 *      8byte[1] : INTEGER <--- NO_CLASS(init), INTEGER(f3)
 *      8byte[2] : INTEGER <--- NO_CLASS(init), INTEGER(f4), INTEGER(f5), INTEGER(f6)
 *      8byte[3] : INTEGER <--- NO_CLASS(init), INTEGER(f7)
 * 5) post merger cleanup:
 *      a) nije ispunjeno
 *      b) nije ispunjeno
 *      c) jeste ispunjeno ---> citav objekat pripada klasi MEMORY
 *      d) nije primenjivo
 *         ---> pojedinacni 8byte objekta se prosledjuju kao da svaki 8byte pripada klasi MEMORY
 *
 */

// Declaring Attributes of Functions
// https://gcc.gnu.org/onlinedocs/gcc-9.2.0/gcc/Function-Attributes.html
// __attribute__ -> koji se abi koristi, implicitno koristimo sysv_abi za linux
void __attribute__((sysv_abi)) foo_3_system_v_abi_explicit(
    char rdi,
    StructType3 stack1PushII,
    int rsi,
    StructType3 stack2PushI,        // najblize vrhu steka je onaj koji je nablizi potpisu funkcije
    long rdx)                       // zbog neodredjenog broja argumenata kod ... u C
{
    /*
     * Prema System V ABI konvenciji (definisano na osnovu atributa funkcije)
     * u skladu sa kojom se vrsi prevodjenje posmatrane funkcije vazi sledece:
     *
     * Inicijalno slobodni GP (general-purpose) registri su: rdi, rsi, rdx, rcx, r8, r9
     * Inicijalno slobodni SSE (streaming simd extension) registri su: xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7
     *
     * Prvi parametar (char rdi) pripada klasi INTEGER
     *     ---> parametar : INTEGER kroz prvi naredni slobodni GP registar (rdi) i alocira citav registar
     *
     * Drugi parametar (StructType3 stack1PushII) prosledjuje svoje 8byte na sledeci nacin
     *     ---> 8byte[0] : MEMORY preko steka na adresi (((unsigned char*) &stack1PushII) + 0 * 8)
     *     ---> 8byte[1] : MEMORY preko steka na adresi (((unsigned char*) &stack1PushII) + 1 * 8)
     *     ---> 8byte[2] : MEMORY preko steka na adresi (((unsigned char*) &stack1PushII) + 2 * 8)
     *     ---> 8byte[3] : MEMORY preko steka na adresi (((unsigned char*) &stack1PushII) + 3 * 8)
     *
     * Treci parametar (int rsi) pripada klasi INTEGER
     *     ---> parametar : INTEGER kroz prvi naredni slobodni GP registar (rsi) i alocira citav registar
     *
     * Cetvrti parametar (StructType3 stack2PushI) prosledjuje svoje 8byte na sledeci nacin
     *     ---> 8byte[0] : MEMORY preko steka na adresi (((unsigned char*) &stack2PushI) + 0 * 8)
     *     ---> 8byte[1] : MEMORY preko steka na adresi (((unsigned char*) &stack2PushI) + 1 * 8)
     *     ---> 8byte[2] : MEMORY preko steka na adresi (((unsigned char*) &stack2PushI) + 2 * 8)
     *     ---> 8byte[3] : MEMORY preko steka na adresi (((unsigned char*) &stack2PushI) + 3 * 8)
     *
     * Peti parametar (long rdx) pripada klasi INTEGER
     *     ---> parametar : INTEGER kroz prvi naredni slobodni GP registar (rdx) i alocira citav registar
     *
     * NAPOMENA: U konkretnom primeru mora biti ispunjeno da je (&stack1PushII < &stack2PushI)
     * odnosno u opstem slucaju za argumente koji se prosledjuju preko steka mora da vazi sledece:
     *     a) argumenti se stavljaju na stek u redosledu zdesna nalevo
     *        (krajnje desni argument, gledno prema potpisu funkcije, prvi se stavlja na stek) odnosno
     *     b) argumenti imaju rastuce vrednosti adresa na steku u redosledu sleva nadesno
     *        (krajnje levi argument, gledano prema potpisu funkcije, bice na najnizoj adresi)
     *
     */
}

void __attribute__((ms_abi)) foo_3_microsoft_abi_explicit(      // ne trazi se na ispitu, samo primer
    char rcx,
    StructType3 mem_pointed_rdx,
    int r8,
    StructType3 mem_pointed_r9,
    long stack)
{
    /*
     * Prema Microsoft ABI konvenciji (definisano na osnovu atributa funkcije)
     * u skladu sa kojom se vrsi prevodjenje posmatrane funkcije vazi sledece:
     *
     * Prvi parametar (char rcx) moze da se prosledi kroz registar
     *     ---> prosledjuje se kroz prvi naredni slobodni GP registar (rcx) i zauzima citav registar
     *
     * Drugi parametar (StructType3 mem_pointed_rdx) ne moze da se prosledi kroz registar
     *     ---> zamenjuje se pokazivacem na prostor gde se nalazi argument
     *          a dati pokazivac se prosledjuje kroz prvi naredni slobodni GP registar(rdx)
     *
     * Treci parametar (int r8) moze da se prosledi kroz registar
     *     ---> prosledjuje se kroz prvi naredni slobodni GP registar (r8) i zauzima citav registar
     *
     * Cetvrti parametar (StructType3 mem_pointed_r9) ne moze da se prosledi kroz registar
     *     ---> zamenjuje se pokazivacem na prostor gde se nalazi argument
     *          a dati pokazivac se prosledjuje kroz prvi naredni slobodni GP registar(r9)
     *
     * Peti parametar (long stack) moze da se prosledi kroz registar
     *     ---> prosledjuje se kroz prvi naredni slobodni GP registar, ali posto su
     *          svi GP registri zauzeti mora da se prosledi preko steka
     *
     */
}

typedef struct
{
    int f1;    // size:  4, alignment: 4
    int f2;    // size:  4, alignment: 4
    double f3; // size:  8, alignment: 8
} StructType4; // size: 16, alignment: 8

/*
 * |        |        |        |        |        |        |        |        |
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |   f3   |   f3   |   f3   |   f3   |   f3   |   f3   |   f3   |   f3   | : 8byte[1]
 * |   f2   |   f2   |   f2   |   f2   |   f1   |   f1   |   f1   |   f1   | : 8byte[0]
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |        |        |        |        |        |        |        |        |
 *
 * gcc classify_argument()
 * https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/i386.cc#L2074
 *
 * System V ABI - 3.2.3 Parameter Passing - Klasifikacija agregatnog argumenta:
 * 1) nije ispunjeno (sizeof(StructType4) = 16)
 * 2) nije primenjivo
 * 3) svaki 8byte se klasifikuje zasebno
 * 4) svako polje se klasifikuje zasebno odnosno rekurzivno (primenom istog ovog algoritma na dato polje)
 *      8byte[0] : INTEGER <--- NO_CLASS(init), INTEGER(f1), INTEGER(f2)
 *      8byte[1] : SSE     <--- NO_CLASS(init), SSE(f3)
 * 5) post merger cleanup:
 *      a) nije ispunjeno
 *      b) nije ispunjeno
 *      c) nije ispunjeno
 *      d) nije ispunjeno
 *         ---> pojedinacni 8byte objekta se prosledjuju prema klasama odredjenim u koraku 4
 *
 */

void foo_4_system_v_abi(
    StructType4 rdi_xmm0)
{
    /*
     * Prema System V ABI konvenciji (podrazumevana konvencija na linux sistemima)
     * u skladu sa kojom se vrsi prevodjenje posmatrane funkcije vazi sledece:
     *
     * Inicijalno slobodni GP (general-purpose) registri su: rdi, rsi, rdx, rcx, r8, r9
     * Inicijalno slobodni SSE (streaming simd extension) registri su: xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7
     *
     * Prvi parametar (StructType4 rdi_xmm0) prosledjuje svoje 8byte na sledeci nacin
     *     ---> 8byte[0] : INTEGER kroz prvi naredni slobodni GP registar (rdi) i alocira citav registar
     *     ---> 8byte[1] : SSE kroz prvi naredni slobodni SSE registar (xmm0) i alocira citav registar
     *
     */
}

typedef struct
{
    int f1;    // size:  4, alignment: 4
    double f2; // size:  8, alignment: 8
    int f3;    // size:  4, alignment: 4
} StructType5; // size: 24, alignment: 8

/*
 * |        |        |        |        |        |        |        |        |
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |  padd  |  padd  |  padd  |  padd  |   f3   |   f3   |   f3   |   f3   | : 8byte[2]
 * |   f2   |   f2   |   f2   |   f2   |   f2   |   f2   |   f2   |   f2   | : 8byte[1]
 * |  padd  |  padd  |  padd  |  padd  |   f1   |   f1   |   f1   |   f1   | : 8byte[0]
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |        |        |        |        |        |        |        |        |
 *
 * gcc classify_argument()
 * https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/i386.cc#L2074
 *
 * System V ABI - 3.2.3 Parameter Passing - Klasifikacija agregatnog argumenta:
 * 1) nije ispunjeno (sizeof(StructType5) = 24)
 * 2) nije primenjivo
 * 3) svaki 8byte se klasifikuje zasebno
 * 4) svako polje se klasifikuje zasebno odnosno rekurzivno (primenom istog ovog algoritma na dato polje)
 *      8byte[0] : INTEGER <--- NO_CLASS(init), INTEGER(f1)
 *      8byte[1] : SSE     <--- NO_CLASS(init), SSE(f2)
 *      8byte[2] : INTEGER <--- NO_CLASS(init), INTEGER(f3)
 * 5) post merger cleanup:
 *      a) nije ispunjeno
 *      b) nije ispunjeno
 *      c) jeste ispunjeno ---> citav objekat pripada klasi MEMORY
 *      d) nije primenjivo
 *         ---> pojedinacni 8byte objekta se prosledjuju kao da svaki 8byte pripada klasi MEMORY
 *
 */

void foo_5_system_v_abi(
    StructType5 stack1PushI)
{
    /*
     * Prema System V ABI konvenciji (podrazumevana konvencija na linux sistemima)
     * u skladu sa kojom se vrsi prevodjenje posmatrane funkcije vazi sledece:
     *
     * Inicijalno slobodni GP (general-purpose) registri su: rdi, rsi, rdx, rcx, r8, r9
     * Inicijalno slobodni SSE (streaming simd extension) registri su: xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7
     *
     * Prvi parametar (StructType5 stack1PushI) prosledjuje svoje 8byte na sledeci nacin
     *     ---> 8byte[0] : MEMORY preko steka na adresi (((unsigned char*) &stack1PushI) + 0 * 8)
     *     ---> 8byte[1] : MEMORY preko steka na adresi (((unsigned char*) &stack1PushI) + 1 * 8)
     *     ---> 8byte[2] : MEMORY preko steka na adresi (((unsigned char*) &stack1PushI) + 2 * 8)
     *
     */
}

typedef struct
{
    int f1;    // size:  4, alignment: 4
    float f2;  // size:  4, alignment: 4
} StructType6; // size:  8, alignment: 4

/*
 * |        |        |        |        |        |        |        |        |
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |   f2   |   f2   |   f2   |   f2   |   f1   |   f1   |   f1   |   f1   | : 8byte[0]
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |        |        |        |        |        |        |        |        |
 *
 * gcc classify_argument()
 * https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/i386.cc#L2074
 *
 * System V ABI - 3.2.3 Parameter Passing - Klasifikacija agregatnog argumenta:
 * 1) nije ispunjeno (sizeof(StructType6) = 8)
 * 2) nije primenjivo
 * 3) svaki 8byte se klasifikuje zasebno
 * 4) svako polje se klasifikuje zasebno odnosno rekurzivno (primenom istog ovog algoritma na dato polje)
 *      8byte[0] : INTEGER <--- NO_CLASS(init), INTEGER(f1), SSE(f2)
 * 5) post merger cleanup:
 *      a) nije ispunjeno
 *      b) nije ispunjeno
 *      c) nije ispunjeno
 *      d) nije primenjivo
 *         ---> pojedinacni 8byte objekta se prosledjuju prema klasama odredjenim u koraku 4
 *
 */

void foo_6_system_v_abi(
    StructType6 rdi)
{
    /*
     * Prema System V ABI konvenciji (podrazumevana konvencija na linux sistemima)
     * u skladu sa kojom se vrsi prevodjenje posmatrane funkcije vazi sledece:
     *
     * Inicijalno slobodni GP (general-purpose) registri su: rdi, rsi, rdx, rcx, r8, r9
     * Inicijalno slobodni SSE (streaming simd extension) registri su: xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7
     *
     * Prvi parametar (StructType6 rdi) pripada klasi INTEGER
     *     ---> parametar : INTEGER kroz prvi naredni slobodni GP registar (rdi) i alocira citav registar
     *
     */
}

typedef struct
{
    float f1; // size:  4, alignment: 4
    struct
    {
        int f1; // size:  4, alignment: 4
        int f2; // size:  4, alignment: 4
    } f2;       // size:  8, alignment: 4
    float f3;   // size:  4, alignment: 4
} StructType7;  // size: 16, alignment: 4

/*
 * |        |        |        |        |        |        |        |        |
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |   f3   |   f3   |   f3   |   f3   | f2.f2  | f2.f2  | f2.f2  | f2.f2  | : 8byte[1]
 * | f2.f1  | f2.f1  | f2.f1  | f2.f1  |   f1   |   f1   |   f1   |   f1   | : 8byte[0]
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |        |        |        |        |        |        |        |        |
 *
 * gcc classify_argument()
 * https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/i386.cc#L2074
 *
 * System V ABI - 3.2.3 Parameter Passing - Klasifikacija agregatnog argumenta:
 * 1) nije ispunjeno (sizeof(StructType7) = 16)
 * 2) nije primenjivo
 * 3) svaki 8byte se klasifikuje zasebno
 * 4) svako polje se klasifikuje zasebno odnosno rekurzivno (primenom istog ovog algoritma na dato polje)
 *      8byte[0] : INTEGER <--- NO_CLASS(init), SSE(f1), INTEGER(f2.f1)
 *      8byte[1] : INTEGER <--- NO_CLASS(init), INTEGER(f2.f2), SSE(f3)
 * 5) post merger cleanup:
 *      a) nije ispunjeno
 *      b) nije ispunjeno
 *      c) nije ispunjeno
 *      d) nije primenjivo
 *         ---> pojedinacni 8byte objekta se prosledjuju prema klasama odredjenim u koraku 4
 *
 */

void foo_7_system_v_abi(
    StructType7 rdi_rsi)
{
    /*
     * Prema System V ABI konvenciji (podrazumevana konvencija na linux sistemima)
     * u skladu sa kojom se vrsi prevodjenje posmatrane funkcije vazi sledece:
     *
     * Inicijalno slobodni GP (general-purpose) registri su: rdi, rsi, rdx, rcx, r8, r9
     * Inicijalno slobodni SSE (streaming simd extension) registri su: xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7
     *
     * Prvi parametar (StructType7 rdi_rsi) prosledjuje svoje 8byte na sledeci nacin
     *     ---> 8byte[0] : INTEGER kroz prvi naredni slobodni GP registar (rdi) i alocira citav registar
     *     ---> 8byte[1] : INTEGER kroz prvi naredni slobodni GP registar (rsi) i alocira citav registar
     *
     */
}

int main()
{
    printf("sizeof(StructType1) = %ld\n", sizeof(StructType1));
    StructType1 struct_type_1_arg = {'a', 0x11111111, 0x1111};
    foo_1_system_v_abi('a', 0x11111111, struct_type_1_arg, (void *)0x1111111111111111, struct_type_1_arg, 0x1111111111111111l);
    /*
     * ...
     * mov    BYTE PTR [rbp-0xbc],0x61
     * mov    DWORD PTR [rbp-0xb8],0x11111111
     * mov    WORD PTR [rbp-0xb4],0x1111
     * mov    rax,QWORD PTR [rbp-0xbc]
     * mov    ecx,DWORD PTR [rbp-0xb4]
     * mov    rdx,rcx
     * sub    rsp,0x10
     * mov    rcx,rsp
     * mov    rsi,QWORD PTR [rbp-0xbc]
     * mov    QWORD PTR [rcx],rsi
     * mov    esi,DWORD PTR [rbp-0xb4]
     * mov    DWORD PTR [rcx+0x8],esi
     * movabs r9,0x1111111111111111
     * movabs r8,0x1111111111111111
     * mov    rcx,rdx
     * mov    rdx,rax
     * mov    esi,0x11111111
     * mov    edi,0x61
     * call   1149 <foo_1_system_v_abi>
     */

    printf("sizeof(StructType2) = %ld\n", sizeof(StructType2));
    StructType2 struct_type_2_arg = {0x22222222, 0x22222222, 0x2222, struct_type_1_arg, 'b', 0x2222};
    foo_2_system_v_abi(struct_type_2_arg);
    /*
     * ...
     * sub    rsp,0x20
     * mov    rax,rsp
     * mov    rcx,QWORD PTR [rbp-0x70]
     * mov    rbx,QWORD PTR [rbp-0x68]
     * mov    QWORD PTR [rax],rcx
     * mov    QWORD PTR [rax+0x8],rbx
     * mov    rdx,QWORD PTR [rbp-0x60]
     * mov    QWORD PTR [rax+0x10],rdx
     * mov    edx,DWORD PTR [rbp-0x58]
     * mov    DWORD PTR [rax+0x18],edx
     * call   1179 <foo_2_system_v_abi>
     */

    printf("sizeof(StructType3) = %ld\n", sizeof(StructType3));
    StructType3 struct_type_3_arg1 = {'c', 'c', 0x3333333333333311l, 0x3333, 'c', 0x33333311, 'c'};
    StructType3 struct_type_3_arg2 = {'c', 'c', 0x3333333333333322l, 0x3333, 'c', 0x33333322, 'c'};
    foo_3_system_v_abi_explicit('c', struct_type_3_arg1, 0x33333333, struct_type_3_arg2, 0x3333333333333333l);
    /*
     * ...
     * push   QWORD PTR [rbp-0x18] # push 8byte[3] of struct_type_3_arg2
     * push   QWORD PTR [rbp-0x20] # push 8byte[2] of struct_type_3_arg2
     * push   QWORD PTR [rbp-0x28] # push 8byte[1] of struct_type_3_arg2
     * push   QWORD PTR [rbp-0x30] # push 8byte[0] of struct_type_3_arg2
     * push   QWORD PTR [rbp-0x38] # push 8byte[3] of struct_type_3_arg1
     * push   QWORD PTR [rbp-0x40] # push 8byte[2] of struct_type_3_arg1
     * push   QWORD PTR [rbp-0x48] # push 8byte[1] of struct_type_3_arg1
     * push   QWORD PTR [rbp-0x50] # push 8byte[0] of struct_type_3_arg1
     * movabs rdx,0x3333333333333333
     * mov    esi,0x33333333
     * mov    edi,0x63
     * call   1184 <foo_3_system_v_abi_explicit>
     */
    foo_3_microsoft_abi_explicit('c', struct_type_3_arg1, 0x33333333, struct_type_3_arg2, 0x3333333333333333l);
    /*
     * ...
     * mov    rax,QWORD PTR [rbp-0x50]
     * mov    rdx,QWORD PTR [rbp-0x48]
     * mov    QWORD PTR [rbp-0xf0],rax  # place 8byte[0] of struct_type_3_arg1 in memory
     * mov    QWORD PTR [rbp-0xe8],rdx  # place 8byte[1] of struct_type_3_arg1 in memory
     * mov    rax,QWORD PTR [rbp-0x40]
     * mov    rdx,QWORD PTR [rbp-0x38]
     * mov    QWORD PTR [rbp-0xe0],rax  # place 8byte[2] of struct_type_3_arg1 in memory
     * mov    QWORD PTR [rbp-0xd8],rdx  # place 8byte[3] of struct_type_3_arg1 in memory
     * mov    rax,QWORD PTR [rbp-0x30]
     * mov    rdx,QWORD PTR [rbp-0x28]
     * mov    QWORD PTR [rbp-0x110],rax # place 8byte[0] of struct_type_3_arg2 in memory
     * mov    QWORD PTR [rbp-0x108],rdx # place 8byte[1] of struct_type_3_arg2 in memory
     * mov    rax,QWORD PTR [rbp-0x20]
     * mov    rdx,QWORD PTR [rbp-0x18]
     * mov    QWORD PTR [rbp-0x100],rax # place 8byte[2] of struct_type_3_arg2 in memory
     * mov    QWORD PTR [rbp-0xf8],rdx  # place 8byte[3] of struct_type_3_arg2 in memory
     * lea    rdx,[rbp-0x110]
     * lea    rax,[rbp-0xf0]
     * sub    rsp,0x8
     * push   0x33333333
     * mov    DWORD PTR [rsp+0x4],0x33333333
     * sub    rsp,0x20
     * mov    r9,rdx
     * mov    r8d,0x33333333
     * mov    rdx,rax
     * mov    ecx,0x63
     * call   119b <foo_3_microsoft_abi_explicit>
     */

    printf("sizeof(StructType4) = %ld\n", sizeof(StructType4));
    StructType4 struct_type_4_arg = {0x44444444, 0x44444444, 4.44};
    foo_4_system_v_abi(struct_type_4_arg);
    /*
     * ...
     * mov    rdx,QWORD PTR [rbp-0xb0] # fetch 8byte[0] of struct_type_4_arg
     * mov    rax,QWORD PTR [rbp-0xa8] # fetch 8byte[1] of struct_type_4_arg
     * mov    rdi,rdx
     * movq   xmm0,rax
     * call   11af <foo_4_system_v_abi>
     */

    printf("sizeof(StructType5) = %ld\n", sizeof(StructType5));
    StructType5 struct_type_5_arg = {0x55555555, 5.55, 0x55555555};
    foo_5_system_v_abi(struct_type_5_arg);
    /*
     * ...
     * sub    rsp,0x8  # the end of the input argument area shall be aligned on a 16
     * push   QWORD PTR [rbp-0x80]
     * push   QWORD PTR [rbp-0x88]
     * push   QWORD PTR [rbp-0x90]
     * call   11ca <foo_5_system_v_abi>
     */

    printf("sizeof(StructType6) = %ld\n", sizeof(StructType6));
    StructType6 struct_type_6_arg = {0x66666666, 6.66f};
    foo_6_system_v_abi(struct_type_6_arg);
    /*
     * ...
     * mov    DWORD PTR [rbp-0xc4],0x66666666
     * movss  xmm0,DWORD PTR [rip+0xbc6]      # fetch floating-point value 6.66f
     * movss  DWORD PTR [rbp-0xc0],xmm0
     * mov    rax,QWORD PTR [rbp-0xc4]
     * mov    rdi,rax
     * call   11d5 <foo_6_system_v_abi>
     */

    printf("sizeof(StructType7) = %ld\n", sizeof(StructType7));
    StructType7 struct_type_7_arg = {7.71f, {0x77777711, 0x77777722}, 7.72f};
    foo_7_system_v_abi(struct_type_7_arg);
    /*
     * ...
     * movss  xmm0,DWORD PTR [rip+0xb95]       # fetch floating-point value 7.71f
     * movss  DWORD PTR [rbp-0xa0],xmm0
     * mov    DWORD PTR [rbp-0x9c],0x77777711
     * mov    DWORD PTR [rbp-0x98],0x77777722
     * movss  xmm0,DWORD PTR [rip+0xb71]       # fetch floating-point value 7.72f
     * movss  DWORD PTR [rbp-0x94],xmm0
     * mov    rdx,QWORD PTR [rbp-0xa0]
     * mov    rax,QWORD PTR [rbp-0x98]
     * mov    rdi,rdx
     * mov    rsi,rax
     * call   11e4 <foo_7_system_v_abi>
     */

    return 0;
}

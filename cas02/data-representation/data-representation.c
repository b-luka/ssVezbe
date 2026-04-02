#include <stdio.h>

typedef struct
{
	char field1;	// size:  1, alignment: 1
	char field2;	// size:  1, alignment: 1
	long field3;	// size:  8, alignment: 8
	short field4; // size:  2, alignment: 2
	char field5;	// size:  1, alignment: 1
	int field6;		// size:  4, alignment: 4
	char field7;	// size:  1, alignment: 1
} StructType;		// size: 32, alignment: 8

typedef struct {
    int x1; // 4
    char x2; // 1
} StructType2;  // size: 5, align: 4; 5 -> 8

typedef struct {
    long x1;    // 8
    char x2;    // 1
    long x3;    // 8
} StructType3;  // 24


typedef struct {
    int x1;    // 4
    char x2;    // 1
    long x3;    // 8
} StructType4;  // 16


typedef struct {
    int x1;    // 4
    int x2;    // 4
    char x3;    // 1
} StructType5;  // 12

typedef struct {
    int x1;    // 4
    int x2;    // 4
    char x3[5];    // 1
} StructType6;  // 12


/*
 * Poravnanje *primitivnih tipova* jednako je njihovoj velicini izrazenoj u broju bajtova.
 *
 * Poravnanje *strukture* jednako je poravnanju njenog polja sa najstrozim poravnanjem.
 * Velicina strukture (sizeof) mora biti celobrojni umnozak njenog poravnanja.
 *
 * Polje field3 tipa long strukture tipa StructType predstavlja njeno polje sa
 * najstrozim poravnanjem (alignment) koje iznosi 8. Prema tome, poravnanje strukture
 * tipa StructType iznosi 8, dok je njena velicina celobrojni umnozak broja 8.
 *
 * Legenda predstave adresnog prostora:
 *
 *     +7       +6       +5       +4       +3       +2       +1       +0
 *
 * | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | <--- address + 0x08
 * | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | <--- address + 0x00
 *
 *
 *
 * Smestanje strukture tipa StructType u adresni prostor:
 *
 * |        |        |        |        |        |        |        |        |
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |  padd  |  padd  |  padd  |  padd  |  padd  |  padd  |  padd  | field7 |
 * | field6 | field6 | field6 | field6 |  padd  | field5 | field4 | field4 |
 * | field3 | field3 | field3 | field3 | field3 | field3 | field3 | field3 |
 * |  padd  |  padd  |  padd  |  padd  |  padd  |  padd  | field2 | field1 | <--- struct_address
 *  -------- -------- -------- -------- -------- -------- -------- --------
 * |        |        |        |        |        |        |        |        |
 *
 */

int main()
{
	StructType struct_object1 = {'a', 'a', 0x1111111111111111l, 0x1111, 'a', 0x11111111, 'a'};
	/*
	 * |        |        |        |        |        |        |        |        |
	 *  -------- -------- -------- -------- -------- -------- -------- --------
	 * | -0x21  | -0x22  | -0x23  | -0x24  | -0x25  | -0x26  | -0x27  |(-0x28) |
	 * |(-0x29  | -0x2A  | -0x2B  | -0x2C) | -0x2D  |(-0x2E) |(-0x2F  | -0x30) |
	 * |(-0x31  | -0x32  | -0x33  | -0x34  | -0x35  | -0x36  | -0x37  | -0x38) |
	 * | -0x39  | -0x3A  | -0x3B  | -0x3C  | -0x3D  | -0x3E  |(-0x3F) |(-0x40) |
	 *  -------- -------- -------- -------- -------- -------- -------- --------
	 * |        |        |        |        |        |        |        |        |
	 *
	 * mov    BYTE PTR [rbp-0x40],0x61
	 * mov    BYTE PTR [rbp-0x3f],0x61
	 * movabs rax,0x1111111111111111
	 * mov    QWORD PTR [rbp-0x38],rax
	 * mov    WORD PTR [rbp-0x30],0x1111
	 * mov    BYTE PTR [rbp-0x2e],0x61
	 * mov    DWORD PTR [rbp-0x2c],0x11111111
	 * mov    BYTE PTR [rbp-0x28],0x61
	 *
	 */

	StructType struct_object2 = {'b', 'b', 0x2222222222222222l, 0x2222, 'b', 0x22222222, 'b'};
	/*
	 * |        |        |        |        |        |        |        |        |
	 *  -------- -------- -------- -------- -------- -------- -------- --------
	 * | -0x01  | -0x02  | -0x03  | -0x04  | -0x05  | -0x06  | -0x07  |(-0x08) |
	 * |(-0x09  | -0x0A  | -0x0B  | -0x0C) | -0x0D  |(-0x0E) |(-0x0F  | -0x10) |
	 * |(-0x11  | -0x12  | -0x13  | -0x14  | -0x15  | -0x16  | -0x17  | -0x18) |
	 * | -0x19  | -0x1A  | -0x1B  | -0x1C  | -0x1D  | -0x1E  |(-0x1F) |(-0x20) |
	 *  -------- -------- -------- -------- -------- -------- -------- --------
	 * |        |        |        |        |        |        |        |        |
	 *
	 * mov    BYTE PTR [rbp-0x20],0x62
	 * mov    BYTE PTR [rbp-0x1f],0x62
	 * movabs rax,0x2222222222222222
	 * mov    QWORD PTR [rbp-0x18],rax
	 * mov    WORD PTR [rbp-0x10],0x2222
	 * mov    BYTE PTR [rbp-0xe],0x62
	 * mov    DWORD PTR [rbp-0xc],0x22222222
	 * mov    BYTE PTR [rbp-0x8],0x62
	 *
	 */

	printf("sizeof(StructType) = %ld\n", sizeof(StructType)); // 32


    StructType2 test = {4, 'a'};
    printf("sizeof(StructType2) = %ld\n", sizeof(StructType2));
    printf("sizeof(StructType3) = %ld\n", sizeof(StructType3));
	printf("sizeof(StructType4) = %ld\n", sizeof(StructType4));
	printf("sizeof(StructType5) = %ld\n", sizeof(StructType5));
    printf("sizeof(StructType6) = %ld\n", sizeof(StructType6));
    return 0;
}

#include <stdio.h>
#include <stdint.h>

typedef struct
{
  int32_t field1;
  int32_t field2;
} ImaginaryType;

extern int32_t a;

extern int32_t *b;

extern int32_t c[];

extern ImaginaryType d;

/*
 * Vrednost *promenljive* (C jezik) prema semantici C programskog jezika,
 * ciji je identifikator jednak identifikatoru nekog *simbola* (asemblerski jezik),
 * nije jednaka vrednosti istoimenog simbola vec
 * jeste jednaka sadrzaju u memoriji na adresi ukazanoj vrednoscu istoimenog simbola
 * (koliko tacno sadrzaja iz memorije treba uzeti definisano je tipom promenljive).
 *
 * Vrednost nekog simbola (adresu memorijske lokacije za koju je dati simbol vezan)
 * moguce je dohvatiti u C programskog jeziku preko istoimene promenljive
 * pomocu operatora za dohvatanje adrese (&)
 */

void main()
{
  printf("\n====================================================\n");
  // vrednost *promenljive* a tipa (int32_t) prema semantici C programskog jezika
  printf("a = (int32_t) mem32[valueOfSymbol(a)] = %d \n", a);
  // vrednost *simbola* a (adresa na kojoj se cuva vrednost *promenljive* a)
  printf("&a = valueOfSymbol(a) = %p \n", &a);

  printf("\n=====================================================\n");
  // vrednost *promenljive* b tipa (int32_t *) prema semantici C programskog jezika
  printf("b = (int32_t *) mem64[valueOfSymbol(b)] = %p \n", b);
  // vrednost *simbola* b (adresa na kojoj se cuva vrednost *promenljive* b)
  printf("&b = valueOfSymbol(b) = %p \n", &b);

  printf("\n=====================================================\n");
  for (int32_t i = 0; i < 10; i++)
  {
    printf("c[%d] = (int32_t) mem32[valueOfSymbol(c) + %d * sizeof(int32_t)] = %d \n", i, i, c[i]);
  }
  printf("&c = valueOfSymbol(c) = %p \n", &c);

  printf("\n=====================================================\n");
  printf("d.field1 = (int32_t) mem32[valueOfSymbol(d)] = %d \n", d.field1);
  printf("d.field2 = (int32_t) mem32[valueOfSymbol(d) + sizeof(d.field1) + alignment(d.field2)] = %d \n", d.field2);
  printf("&d = valueOfSymbol(d) = %p \n\n", &d);
}

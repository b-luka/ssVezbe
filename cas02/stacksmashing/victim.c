#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void worm_foo();

/*
 *
 *             HIGH ADDRESSES
 * 
 * 
 *          | ???????????????? |   0x7fffffffde18: [rbp + 0x18] (previous frame)
 *          | ???????????????? |   0x7fffffffde10: [rbp + 0x10] (previous frame)
 * ------------------------------------------------------------------------------------
 *          | 00005555555551fc |   0x7fffffffde08: [rbp + 0x08] (return address)
 *  RBP --> | 00007fffffffde30 |   0x7fffffffde00: [rbp + 0x00] (old rbp value)
 *          | 0000000000000111 |   0x7fffffffddf8: [rbp - 0x08] (stack_variable_1)
 *          | 0000000000000222 |   0x7fffffffddf0: [rbp - 0x10] (stack_variable_2)
 *          | aaaaaaaaaaaaaaaa |   0x7fffffffdde8: [rbp - 0x18] (stack_buffer[15-08])
 *          | aaaaaaaaaaaaaaaa |   0x7fffffffdde0: [rbp - 0x20] (stack_buffer[07-00])
 *          | 00005555555592a0 |   0x7fffffffddd8: [rbp - 0x28] (param)
 *  RSP --> | 0000000000000010 |   0x7fffffffddd0: [rbp - 0x30] (param_length)
 *
 * 
 *              LOW ADDRESSES
 */

void service_foo(char *param, long param_length)
{
  long stack_variable_1 = 0x111;
  char stack_buffer[16];
  long stack_variable_2 = 0x222;

  // ASLR (Address Space Layout Randomization)
  printf("&stack_buffer = %p\n", &stack_buffer);
  printf("&worm_foo     = %p\n", &worm_foo);

  printf(
    "\nbefore memcpy() :"
    "\nstack_variable_1 = %#lx"
    "\nstack_variable_2 = %#lx\n",
    stack_variable_1,
    stack_variable_2
  );

  // some mocked processing: begin
  memcpy(stack_buffer, param, param_length);
  // some mocked processing: end

  printf(
    "\nafter memcpy() :"
    "\nstack_variable_1 = %#lx"
    "\nstack_variable_2 = %#lx\n",
    stack_variable_1,
    stack_variable_2
  );
}

void worm_foo()
{
  printf("\n***** worm_foo() *****\n");
  exit(0);
}
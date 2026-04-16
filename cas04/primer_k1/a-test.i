# 0 "test.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "test.c"
# 16 "test.c"
typedef struct {
    int array[16];
} StructType1;

extern StructType1 bar(int a1, int a2, int a3, int a4);

int foo(StructType1 arg1) {
    StructType1 local = bar(2, arg1.array[4], 9, arg1.array[13]);


    return local.array[11] + local.array[7];
}

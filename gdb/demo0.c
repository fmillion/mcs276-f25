#include <stdio.h>

int add(int a, int b) {
    int result = a + b;
    return result;
}

int main() {
    int x = 5;
    int y = 10;
    int sum = add(x, y);
    printf("Sum: %d\n", sum);
    return 0;
}

/*
 * Steps
 *
 * 1. gcc -g -O0 -o demo0 demo0.c
 * 2. gdb ./demo0
 * 3. (gdb) break main
 * 4. (gdb) run
 * 5. (gdb) step # until inside add
*/
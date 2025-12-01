/*
 * demo1_basic.c
 * MCS 276 - GDB Demonstration Program #1
 * 
 * Purpose: A simple program to demonstrate basic GDB commands.
 * This program is intentionally simple so students can focus on
 * learning GDB rather than understanding complex code.
 *
 * Compile: gcc -g -o demo1 demo1_basic.c
 * Debug:   gdb ./demo1
 */

#include <stdio.h>

/* 
 * A simple function to demonstrate stepping into functions
 * and observing the stack.
 */
int add_numbers(int a, int b) {
    int result = a + b;
    return result;
}

/*
 * Another function to create a deeper call stack.
 */
int compute_sum(int x, int y, int z) {
    int partial = add_numbers(x, y);
    int total = add_numbers(partial, z);
    return total;
}

int main() {
    // Simple local variables - good for demonstrating 'info locals'
    int first = 10;
    int second = 20;
    int third = 15;
    
    // Function call - good for demonstrating 'step' vs 'next'
    int result = compute_sum(first, second, third);
    
    // Output
    printf("The sum of %d, %d, and %d is: %d\n", 
           first, second, third, result);
    
    return 0;
}

/*
 * Steps
 * 
 * 1. Start GDB:
 *    $ gdb ./demo1
 *
 * 2. Set a breakpoint at main:
 *    (gdb) break main
 *
 * 3. Run the program:
 *    (gdb) run
 *
 * 4. Show local variables (note: some may be uninitialized):
 *    (gdb) info locals
 *
 * 5. Step through a few lines:
 *    (gdb) next
 *    (gdb) next
 *    (gdb) info locals     <- Now variables have values!
 *
 * 6. When you reach compute_sum(), use 'step' to enter it:
 *    (gdb) step
 *
 * 7. Show the backtrace:
 *    (gdb) backtrace
 *    You'll see: compute_sum called from main
 *
 * 8. Step into add_numbers():
 *    (gdb) step
 *    (gdb) backtrace
 *    Now you'll see three frames: add_numbers, compute_sum, main
 *
 * 9. Show registers:
 *    (gdb) info registers
 *    Point out: rdi/x0 contains first argument, rsi/x1 contains second
 *
 * 10. Use 'finish' to return from add_numbers():
 *     (gdb) finish
 *     Note: GDB shows the return value
 *
 * 11. Continue to end:
 *     (gdb) continue
 */

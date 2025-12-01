/*
 * demo2_memory.c
 * MCS 276 - GDB Demonstration Program #2
 * 
 * Purpose: Demonstrate memory examination with GDB.
 * Shows how to use the 'x' command to examine arrays,
 * strings, and multi-byte integers.
 *
 * Compile: gcc -g -o demo2 demo2_memory.c
 * Debug:   gdb ./demo2
 */

#include <stdio.h>
#include <stdint.h>

int main() {
    // An array of integers - good for examining memory layout
    int numbers[5] = {10, 20, 30, 40, 50};
    
    // A multi-byte integer - good for demonstrating endianness
    uint32_t big_number = 0xDEADBEEF;
    
    // A 16-bit integer for comparison
    uint16_t small_number = 0xCAFE;
    
    // A string - good for examining character data
    char greeting[] = "Hello, GDB!";
    
    // A pointer to the string
    char *str_ptr = greeting;
    
    // Print addresses so students can see them in output too
    printf("Address of numbers[0]: %p\n", (void*)&numbers[0]);
    printf("Address of big_number: %p\n", (void*)&big_number);
    printf("Address of greeting:   %p\n", (void*)greeting);
    
    printf("\nValues:\n");
    printf("big_number = 0x%X\n", big_number);
    printf("small_number = 0x%X\n", small_number);
    printf("greeting = \"%s\"\n", greeting);
    
    return 0;
}

/*
 * Steps:
 * 
 * 1. Start and break at main:
 *    $ gdb ./demo2
 *    (gdb) break main
 *    (gdb) run
 *
 * 2. Step past the variable declarations:
 *    (gdb) next
 *    (repeat until past the char *str_ptr line)
 *
 * 3. Examine the integer array:
 *    (gdb) print numbers
 *    (gdb) x/5dw &numbers[0]     <- 5 words, decimal format
 *    (gdb) x/20xb &numbers[0]    <- 20 bytes, hex format
 *    
 *    Point out: Each integer takes 4 bytes. You can see the
 *    individual bytes and how they're stored in little-endian order.
 *
 * 4. Examine big_number to see endianness:
 *    (gdb) print/x big_number     <- Shows 0xDEADBEEF
 *    (gdb) x/4xb &big_number      <- Shows individual bytes
 *    
 *    IMPORTANT: On a little-endian system (x86, most ARM), you'll see:
 *    0xef 0xbe 0xad 0xde
 *    
 *    This is the "DEADBEEF" stored least-significant-byte first!
 *    This directly connects to the endianness lecture.
 *
 * 5. Compare with small_number:
 *    (gdb) print/x small_number   <- Shows 0xCAFE
 *    (gdb) x/2xb &small_number    <- Shows: 0xfe 0xca
 *
 * 6. Examine the string:
 *    (gdb) print greeting          <- Shows the string with quotes
 *    (gdb) x/s greeting            <- Also shows as string
 *    (gdb) x/12xb greeting         <- Shows individual bytes (ASCII values!)
 *    
 *    Point out: 'H' is 0x48, 'e' is 0x65, etc. 
 *    The last byte is 0x00 - the null terminator!
 *
 * 7. Examine via the pointer:
 *    (gdb) print str_ptr           <- Shows the address
 *    (gdb) print *str_ptr          <- Shows just 'H' (first char)
 *    (gdb) x/s str_ptr             <- Shows the full string
 *    
 *    Point out: The pointer itself holds an address, and we can
 *    examine memory at that address.
 *
 * 8. Look at addresses:
 *    (gdb) print &numbers[0]
 *    (gdb) print &numbers[1]
 *    
 *    Point out: The addresses differ by 4 (sizeof(int)).
 *    This is pointer arithmetic in action!
 */

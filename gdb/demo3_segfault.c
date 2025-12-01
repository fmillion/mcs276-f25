/*
 * demo3_segfault.c
 * MCS 276 - GDB Demonstration Program #3
 * 
 * Purpose: A program with a SEGMENTATION FAULT for debugging demonstration.
 * This shows how to use GDB to diagnose a crash.
 *
 * Compile: gcc -g -o demo3 demo3_segfault.c
 * 
 * IMPORTANT: This program WILL crash! That's intentional.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This function has a bug: it dereferences a NULL pointer.
 */
void process_data(int *data, int count) {
    int sum = 0;
    
    // Bug: We don't check if data is NULL!
    for (int i = 0; i < count; i++) {
        sum += data[i];  // CRASH happens here if data is NULL
    }
    
    printf("Sum: %d\n", sum);
}

/*
 * This function sometimes returns NULL, simulating a failed operation.
 */
int* get_data(int should_fail) {
    if (should_fail) {
        // Simulate a failure condition
        return NULL;
    }
    
    // Allocate and return some data
    int *data = malloc(5 * sizeof(int));
    if (data != NULL) {
        for (int i = 0; i < 5; i++) {
            data[i] = (i + 1) * 10;  // 10, 20, 30, 40, 50
        }
    }
    return data;
}

int main() {
    printf("Attempting to process data...\n");
    
    // First call works fine
    int *good_data = get_data(0);  // 0 = don't fail
    if (good_data != NULL) {
        process_data(good_data, 5);
        free(good_data);
    }
    
    printf("First processing complete.\n\n");
    
    // Second call will get NULL data
    printf("Attempting second processing...\n");
    int *bad_data = get_data(1);   // 1 = simulate failure
    
    // BUG: We forgot to check if bad_data is NULL!
    process_data(bad_data, 5);  // This will crash!
    
    // This line will never be reached
    printf("All done!\n");
    
    return 0;
}

/*
 * Steps:
 * 
 * 1. Run the program normally first to see the crash:
 *    $ ./demo3
 *    You'll see "Segmentation fault"
 *
 * 2. Now run under GDB:
 *    $ gdb ./demo3
 *    (gdb) run
 *    
 *    GDB will catch the crash and show where it happened.
 *    You should see something like:
 *    "Program received signal SIGSEGV, Segmentation fault."
 *
 * 3. See where you are:
 *    (gdb) backtrace
 *    
 *    Output will show:
 *    #0  process_data (data=0x0, count=5) at demo3_segfault.c:22
 *    #1  main () at demo3_segfault.c:56
 *    
 *    KEY INSIGHT: data=0x0 means data is NULL (address 0)!
 *
 * 4. Look at the code:
 *    (gdb) list
 *    
 *    This shows the source around the crash point.
 *
 * 5. Examine the variables:
 *    (gdb) print data
 *    Output: $1 = (int *) 0x0
 *    
 *    This confirms data is NULL.
 *
 * 6. Try to access what data points to:
 *    (gdb) print *data
 *    Output: Cannot access memory at address 0x0
 *    
 *    GDB tells us exactly why we crashed - we tried to 
 *    read from address 0!
 *
 * 7. Look at the caller to understand why:
 *    (gdb) frame 1
 *    (gdb) list
 *    (gdb) print bad_data
 *    
 *    We can see that main() passed a NULL pointer to process_data().
 *
 * 8. Examine registers at crash point:
 *    (gdb) frame 0
 *    (gdb) info registers
 *    
 *    On x86-64, you'll see rdi = 0 (first argument = data = NULL)
 *
 * THE FIX:
 * Add a NULL check in process_data():
 *    if (data == NULL) {
 *        printf("Error: NULL data pointer\n");
 *        return;
 *    }
 * 
 * OR check the return value in main():
 *    if (bad_data == NULL) {
 *        printf("Error: Failed to get data\n");
 *        return 1;
 *    }
 */

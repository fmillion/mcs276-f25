# Assignment #5: Assembly Subroutines

For this assignment you will use [Simple 8-Bit Assembly Simulator](https://schweigi.github.io/assembler-simulator/) implement an assembly language *subroutine* that performs the `rot13` operation on a null-terminated string **in-place**. Your function will be called by a test harness (code given below) that sets up the input and displays the result.

## Specifications

The `rot13` function is what is known as a Caesar cipher, or sometimes simply a rotation cipher. The idea is that you shift each letter forward by 13 positions, wrapping around to the beginning. 

The letter `A` becomes the letter `N`, because `N` is 13 letters after `A`. Letters are still in sequential order - `B` = `O`, `C` = `P`, and so on. After `M` = `Z`, we wrap around, so input of `N` becomes `A`.

> - A → N, B → O, C → P, ... M → Z
> - N → A, O → B, P → C, ... Z → M

**Spaces, numbers, symbols and any other non-letters remain unchanged.**

In other words, **rot13 is a symmetric encryption cipher - the same operation will encode and decode.**

For example: the string `HELLO` would encode in rot13 as `URYYB`.

### Input and Output

Your function must assume the following when it is called:

- **Register B**: Will contain the memory address of a null-terminated string
- **Register A**: At return time, must contain the **number of characters** that were transformed, **not** including the null-terminating character.
- The string at the input address should be modified in-place
- **String constraints** - You can assume the following to always be true 
  - Maximum 15 characters + null terminator (16 bytes total)
  - Contains only uppercase letters (A-Z) and spaces
  - Properly null-terminated

### Function Label

Your function **must** be labeled as `rot13` (all lowercase). Your code should start with `rot13:`.

To test your code, simply paste your code as is to the end of the test harness and run it in Simple 8-Bit Assembler Simulator.

## Test Harness

The following is the code that you will test with. To test, just add your code to the end.

```assembly
; Test Harness - DO NOT MODIFY
    MOV B, testString      ; Load address of string
    CALL rot13             ; Call your function
    
    ; Display result (copies from address in A to output region)
    MOV C, 0xE0
    MOV D, A
displayLoop:
    MOV A, [D]
    CMP A, 0
    JZ done
    MOV [C], A
    INC C
    INC D
    CMP C, 0xF0           ; Max 16 characters
    JZ done
    JMP displayLoop
done:
    HLT

testString: DB "HELLO WORLD", 0

; Put your code below this line, starting with  "rot13:"
```

## Your Task

Implement the `rot13:` subroutine. Your code should:

1. Read characters from the string at address in register B
2. Transform each letter using `rot13`
3. Write the transformed character back to the same memory location
4. Continue until the null terminator (0) is reached
5. Return the **number of characters transformed** in register A.
6. Use RET to return to the test harness

## Implementation Hints

- You'll need to loop through the string character by character
- Use pointer arithmetic or increment your pointer register
- To check if a character is a letter: compare with ASCII values (A=65 or 0x41, Z=90 or 0x5A)
- For `rot13` logic, consider: if letter is A-M (65-77), add 13; if N-Z (78-90), subtract 13
- Don't forget to preserve the null terminator!
- Remember to set register A before returning

## Grading Rubric

<table><tr><th>Item</th><th>Points</th><th>Notes</th></tr>
<tr><th>Correctness</th><td>60%</td><td>

* Returns correct address in register A (10%)
- Properly transforms A-Z letters using ROT13 (30%)
- Leaves spaces and null terminator unchanged (10%)
- Works correctly with various test strings (10%)

</td></tr>

<tr><th>Code Quality</th><td>25%</td><td>

- Reasonable efficiency - no obviously wasteful operations (10%)
- Clear, logical code structure (15%)

</td></tr>
<tr><th>Documentation</th><td>10%</td><td>

- Algorithm explanation in comments (10%)

</td></tr></table>

## Submission Requirements

Submit a single file containing only your `rot13:` subroutine

This assignment will be due on **Monday, November 24th** at **11:59 PM**. We will discuss this assignment in class on 11/24 - if you are completely stuck, come with questions that day!

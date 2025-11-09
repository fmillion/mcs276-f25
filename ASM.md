# Notes on Assembly Language

## Why Assembly Still Matters

Look at this very simple C program:

```c
int x = 5;
int y = 10;
int z = x + y;
printf("%d", z);
```

This can take ~50 assembly instructions on a real processor when you include all the setup! Let's see why...

* Assembly is the "true instructions" that computers can actually do.
* Especially in RISC systems, these instructions are actually very limited!
  * CISC systems do include more complex instructions, but even those instructions are not high-level constructs we are uesd to.
  * For example, no assembly language in existence has a *native* string type - strings are *always* manipulated in loops or iterations!
* Every single line of C is compiled to one or *many* assembly language instructions!
* Compiler design is an entire sub-discpline of computer science - it focuses on how compilers understand your code, how they map it into assembly instructions, and (importantly) how they *optimize* those instructions for performace, memory usage, etc.
* Understanding assembly helps you:
  * Debug performance problems
  * Understand security vulnerabilities (buffer overflows!)
  * Appreciate what compilers do for you
  * Write better high-level code

Let's see the instructions that we'd need to do the above in Simple 8-Bit Assembler Simulator:

```asm
; int x = 5;
; We store x at memory address 0x80
        MOV A, 5        ; Load immediate value 5 into accumulator
        MOV [0x80], A   ; Store to memory location 0x80 (this is 'x')

; int y = 10;
; We store y at memory address 0x81
        MOV A, 10         ; Load immediate value 10 into accumulator
        MOV [0x81], A     ; Store to memory location 0x81 (this is 'y')

; int z = x + y;
; We store z at memory address 0x82
        MOV A, [0x80]   ; Load x from memory into accumulator
        ADD A, [0x81]   ; Add y from memory to accumulator (result in A)
        MOV A, [0x82]   ; Store result to memory location 0x82 (this is 'z')

; printf("%d", z);
; We need to convert the decimal value in z to ASCII characters.
; For a value like 15, we need to output '1' then '5'

; First, extract the tens digit by dividing by 10
; Since we don't have DIV, we'll subtract 10 repeatedly
        MOV B, 0        ; B will count how many tens
divide_loop:
        SUB B, 10       ; Subtract 10 from accumulator
        JC done_tens    ; If carry flag is set (went negative), we're done; jump to done_tens
        INC B           ; Increment tens counter
        JMP divide_loop ; Continue loop

done_tens:
        ADD A, 10       ; Add back the 10 (we subtracted one too many)
; Now A contains ones digit (5), B contains tens digit (1)

; Output tens digit
        PUSH A          ; Save ones digit
        MOV A, B        ; Get tens digit
        ADD A, 0x30     ; Convert to ASCII ('0' is ASCII 0x30 or decimal 48)
        MOV [0xE8], A   ; Write to memory-mapped console
        
; Output ones digit
        POP A           ; Restore ones digit
        ADD A, 0x30     ; Convert to ASCII
        MOV [0xE9], A   ; Write to memory-mapped console

        HLT             ; End program
```

That's a lot of assembly just to add two numbers and print the result! But this is exactly what is going on inside the computer - and it's what compilers are designed to create from your high-level C code. 

## Common Patterns Workshop

### Pattern 1: Swap Two Values

What if we need to swap two values in memory?

```asm
; Swap the values at address 100 and 101.
; Without a temp variable - use the stack!
MOV A, [100]   ; Load first value
PUSH A         ; Save it
MOV A, [101]   ; Load second value
MOV [100], A   ; Store in first location
POP A          ; Retrieve first value
MOV [101], A   ; Store in second location
```

### Pattern 2: Find Maximum

How might we figure out the *maximum* from a set of numbers stored in memory?

```assembly
; Find max of three numbers at addresses 100, 101, 102
MOV A, [100]   ; Assume first is max
MOV B, [101]   
CMP A, B       ; Compare with second
JNC skip1      ; Jump if A >= B
MOV A, B       ; B is bigger, update max
skip1:
MOV B, [102]
CMP A, B       ; Compare with third
JNC skip2      ; Jump if A >= B
MOV A, B       ; B is bigger, update max
skip2:
MOV [103], A   ; Store result
```

### Pattern 3: Simple Counter Loop

You've definitely used `for n in range(x)` before. How might we do a similar loop in assembly - where a piece of code runs a fixed number of times?

```assembly
; Process exactly N items
MOV C, 5       ; Counter = 5
MOV B, 0       ; Index = 0
process_loop:
  ; Do something with [100 + B]
  MOV A, [100 + B]
  ; ... process it ...
  INC B        ; Next index
  DEC C        ; Count down
  JNZ process_loop
```

> Hint: If we wanted to have a `break` in the loop, we could use an unconditional `JMP` instruction to achieve that!

## Things That Will Blow Your Mind

### Revelation 1: Arrays Don't Exist

Assembly has no concept of arrays! In assembly, we're working with raw memory - so it's up to us to actually implement the abstraction ourselves!

In memory, an array can be simply stored as a series of numbers in order:

```assembly
; Populate an "array" of four values in memory, starting at address 100
MOV [100], 0
MOV [101], 4
MOV [102], 8
MOV [103], 10

; Let's say we want value 4 (index 3).
MOV B, 3           ; i = 3
MOV A, [100 + B]   ; Load from base_address + index

; The CPU just does math on addresses!
```

### Revelation 2: Multiplication by Powers of 2

If we want to **multiply or divide by a power of 2**, we don't need to actually do any real math!

That's because we can **shift bits** in a register - this means, for example, if register A contained the value 5 (`00000101` in binary), we could shift it *left* by two bits to get `00010100`, or `20`. It just so happens that `5 * 4 = 20`, and 2<sup>2</sup> = 4!

```assembly
; Multiply A by 8 the slow way:
MOV B, 5
MUL B, 4       ; Expensive!

; Multiply A by 8 the fast way:
MOV A, 5
SHL A, 2       ; just shift the bits left by 2
```

### Revelation 3: Boolean Logic Without IFs

```assembly
; Set C to 1 if A equals B, 0 otherwise
MOV C, 0       ; Assume false
CMP A, B
JNZ not_equal
MOV C, 1       ; Set true
not_equal:
; No "boolean" type - just numbers!
```

## When Things Go Wrong

Common bugs and fixes:

1. **Infinite Loop**
   ```assembly
   loop:
     ; Forgot to modify condition!
     JMP loop  ; Runs forever
   ```
   *Fix: Always modify your loop variable*

2. **Off-by-One**
   ```assembly
   MOV C, 4
   loop:
     ; Process...
     DEC C
     JNZ loop  ; Runs 4 times, not 5!
   ```
   *Fix: Trace through with small numbers*

3. **Register Reuse**
   ```assembly
   MOV A, [100]  ; Load important value
   CALL function ; Function changes A!
   ADD A, 5      ; A is not what you think!
   ```
   *Fix: Save registers or use different ones*

### Interactive Exercise - Let's Build Something Together

**Challenge: Count how many numbers in an array are greater than 10**

```assembly
; Data at 100-104: [5, 12, 8, 15, 3]
; Result at 105

start:
    MOV D, 0      ; Count = 0
    MOV B, 0      ; Index = 0
    
check_loop:
    MOV A, [100 + B]  ; Load array[index]
    CMP A, 10         ; Compare with 10
    JC not_greater    ; Jump if A < 10
    INC D             ; It's greater! Count++
    
not_greater:
    INC B             ; Next index
    CMP B, 5          ; Done all 5?
    JNZ check_loop    ; If not, continue
    
    MOV [105], D      ; Store count
    HLT
```

# Quick Reference Card

```
S8AS SURVIVAL GUIDE
==================

SUGGESTED MEMORY MAP:
0-127 (0x00 - 0x7F): Your code
128-191 (0x80 - 0xBF): Your data  
192-231 (0xC0 - 0xE7): Stack space
232-255 (0xE8 - 0xFF): Memory-mapped console

REGISTER USES (Convention):
A: Accumulator (math results)
B: Base/Index (array access)
C: Counter (loops)
D: Data (temporary storage)

COMMON PATTERNS:
- Loop N times: MOV C,N / loop: ... / DEC C / JNZ loop
- Array access: MOV A,[base + B] where B is index
- Even check: AND A,1 / JZ even
- Save value: PUSH A / ... / POP A

DEBUGGING TIPS:
1. Step through, don't run
2. Watch register values
3. Check memory after each section
4. Add OUT statements to trace flow
```

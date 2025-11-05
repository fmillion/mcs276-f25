# Programming Lab #3 (Assembly Language)

In this assignment you will explore assembly language fundamentals using the [Simple 8-Bit Assembler Simulator](https://schweigi.github.io/assembler-simulator/).

The Simple 8-Bit Assembler Simulator implements a significantly simplified variant of x86 assembly language - the type of code that runs on a typical Windows PC with an Intel or AMD processor. 

The Simple 8-Bit Assembler Simulator implements a minimal system that has 256 bytes of RAM, which is configured in a von Neumann architecture (instructions and data are held in the same memory). Its CPU includes four general purpose regusters (A through D), as well as a program counter or instruction pointer (IP), the carry flag (C), a zero flag (Z), and an additional F (failure) flag. The F flag signals an exception at the processor level (e.g. dividing by zero).

To use S8AS, you write your assembly language code in the panel on the left. Then, you click the "Assemble" button to *assemble* (not compile) your program. You can then run the program or single-step through it using the controls at the top of the page. 

Some key points to remember:

* Use `DB` to store arbitrary data inside the program, such as strings, integers, etc. Strings are contained within double-quotes, while integers are written bare. Data included with `DB` is stored at the literal location as the data in the assembler program stream - i.e. look at the example program and notice how its first instruction is `JMP start`, which is immediately followed by the data - the assembled output will start with the `JMP` instruction, followed by the actual data stored verbatim.
* Use a string followed by a colon to indicate a **label**. (e.g. `start:`) A label marks a particular position in the program that can be later referenced by label, such as in `JMP` or `MOV` instructions. The label's actual numeric value (the address in RAM of the code or data at that label) is computed during assembly, and any use of the label in code is replaced with the computed value. (This is one of the advantages of using an assembler - even at this low-level of code, the assembler can help us keep our code and data straight!)
* Use the [instruction set reference](https://schweigi.github.io/assembler-simulator/instruction-set.html) provided on the site for information about all of the supported instructions and what they each do. 

## Part 1: Tracing Assembly by Hand

Copy and paste the following program into S8AS:

```
MOV A, 5
MOV B, 3
loop:
  ADD A, B
  DEC B
  JNZ loop
MOV [200], A
HLT
```

1. Trace the program execution - create a table that shows the value of the `A`, `B`, `IP`, `C` and `Z` registers at *each instruction*. Note that there is a loop - you should trace *each step, including each instruction in each loop iteration*! 
2. Note the final result that appeared in address 200 (`0xC8`).
3. **Explain in English what this program did.**

## Part 2: Coding

You will write a simple **passcode rule validator** using assembly.

* Store a *four-digit passcode* in memory using four `DB` statements. Remember to label the first line so you can refer to the passcode's memory address in your code! Store the passcode as *integers* (i.e. `DB 5`, not `DB "5"`) so that you can perform math on the values.
* Write a program that **determines if the four digits of the passcode conform to the following rules:**
    * The *sum of the digits* must be even.
    * At least *one digit* must be greater than or equal to 5.
* If the passcode fulfills the given rules, **write the value `0x59` to the memory location `0xE8` - this should make the letter `Y` appear on the console display.
* If the passcode *does not fulfill* the rules, **write the value `0x4E` to the memory location `0xE8` - this should make the letter `N` appear on the console display.
* **Stop your program by using the `HLT` instruction** - not doing so will cause execution to continue into "uncharted memory" - which is all but guaranteed to "crash" your simulated computer!

Important points:

* Remember that if you need to store data in memory, you must *first declare the "variable"* using a label and the `DB` statement. You must initialize the memory location to some value - usually 0. For example, if you wanted to create a variable called `total`, you'd want to include `total: DB 0` somewhere in your code. 
* Put all of your data values either at the very top (preceded by a `JMP` instruction - see the example Hello World program) or at the very bottom of your code.
* **Hint:** For computing the modulus of a value, if you are computing the modulus of a power of 2 (e.g. `myValue % 2`) you can use either bit-shifting operations or Boolean functions to compute the modulus without needing to use division.

## Extra Credit Opportunity

You can earn up to **10 extra credit points** if you are able to, within the available memory space, write an *error code* into the memory location `0xE9` *in the instance that the passcode validation fails:*

* `0x31` (ASCII character `1`) - The passcode digits do not sum to an even number
* `0x32` (ASCII character `2`) - The passcode does not contain at least one digit greater than 5

# Deliverable

1. A short document (either written and scanned or typed) for Part 1 of the assignment
2. The code you ran in Simple 8-Bit Assembler Simulator to implement Part 2 of the assignment, submitted in a text file.

# Scoring Rubric 

| Item | Points | Notes |
|-|-|-|
| Execution Analysis | 25 | Execution analysis shows correct state of the processor after each step and correctly identifies the operation of the program |
| Passcode Verifier | 75 | The program must meet the requirements given in Part 2. |

This assignment is worth 100 points.
# Programming Lab #1 - Intro to C

For this assignment, you will write a very simple application in Python *and* in C. The application will read a number input by the user, and will print that number in both *hexadecimal* and *binary* format.

## Specifications

Your program MUST adhere to these specifications:

* You must use the repeated division and modulus method we learned in class for converting an integer into binary.
* To convert an arbitrary numeric value into hexadecimal, use modulus 16 and a character array. (e.g. `char hexDigits[] = "0123456789abcdef";` or `char hexDigits[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};`)
* You can expect that your program will receive exactly one valid positive integer in the range allowed by a 32-bit signed integer. You can use the `int` data type on any modern platform to receive this input. You do *not* need to error-check the user's input.

## Required Input and Output

* When run, the program must print `"Input a number: "` - not including the quotes, but *including* the space. It must *not* advance to the next line; the user's input should appear on the same line.
* The program must then wait for the user to input an integer.
* The program must then print two lines, in this exact order:
  * `"Binary: <number_binary>"`
  * `"Hexadecimal: 0x<number_hex>"`
* The hexadecimal number should use *lowercase* characters.

### Optional Enhancement

There are multiple methods for creating your output. Some iterative methods will result in leading 0's. For example, since we know we have 32 bits, we would need a maximum of 8 hexadecimal digits. Some methods for converting will leave you with leading 0's - e.g. "`0x000004FC`". (There *are* methods that don't result in this organically - if you're ambitious, try finding a strategy that doesn't result in leading `0`'s! Hint: *precalculate* the size needed).

It is OK to leave the leading zeroes on your response. However, if you would like extra credit, you can investigate how to use *pointer math* to *move the starting pointer of the string to the first non-`0` character* (OR find a method for conversion that does not result in leading `0`'s to begin with). I leave this as an exercise to you, but I'll present the solution after the assignment due date.

If you're using pointer math: Hint - Use `char *someVariable = outputString` to be able to do pointer math (such as adding or subtracting the pointer's numeric memory location).

# Scoring Rubric

| Item | Points | Notes |
|-|-|-|
| Functional program | 75 | Program produces correct values for output and uses the specified algorithms and techniques. Point loss varies based on incorrect implementation |
| Output format is exactly as required | 25 | The program must print the exact strings given with the correct values. |
| **Extra Credit**: Ensure your output never produces any leading 0's. (e.g. `0x0000ABCD` has leading zeroes, while `0xABCD` does not) | 15 | See the hints in the assignment for advice!

This assignment is worth 100 points, with 15 extra credit points possible.

# A few hints

* The command to *compile* a C program is `gcc -o <program_executable_name> <source_file.c>`. For example: `gcc -o a2 assignment2.c`.
* To run your program, remember to use `./a2` (or whatever you named your executable). On Linux/UNIX, not including the `./` will fail because, unlike Windows, UNIX does *not* look in the current directory for executables. By adding `./` you are explicitly stating "the executable program is in the current directory", which overrides this behavior.
* In C, you can make *copies* of scalar types (i.e. simple numbers) by using equality, e.g. `int copyOfNum = num;`. You may need to use this to create two "accumulators" (or reset one accumulator!) in order to do both conversions.
  * But remember, you *can't* copy more complex types - like arrays and strings - in the same way! We'll talk about this more as we dive deeper into pointers.
* A character array doesn't need to end with a null character (`\0`) if you are not using it as a string. An array of characters being used only for indexing into those characters (hint: for doing hexadecimal conversion) need only contain exactly the elements needed (e.g. `char[16]`).
* Remember that, just like Python, strings (and character arrays) in C can be indexed. You'll find this comes in handy when converting to hexadecimal!
* Remember to use `\n` where necessary (and *not* use it where not necessary!). Extra line breaks or missing line breaks are considered incorrect output and may result in some minor point loss.

The following are example runs of a correct implementation of the program. The output of your program must look *IDENTICAL* to the following given the input shown and not trimming leading `0`'s:

```
[fmillion@lenovo solutions]$ ./a2
Input a number: 1234
Binary: 00000000000000000000010011010010
Hexadecimal: 0x000004d2
[fmillion@lenovo solutions]$ ./a2
Input a number: 0
Binary: 00000000000000000000000000000000
Hexadecimal: 0x00000000
[fmillion@lenovo solutions]$ ./a2
Input a number: 1234567890
Binary: 01001001100101100000001011010010
Hexadecimal: 0x499602d2
```

> [!TIP]
> You do not have to name your executable `a2`, nor do you need to submit your executable - only your C source file needs to be submitted.
>
> You can choose any name to use for the executable while you are testing. 
>
> Your Linux prompt will likely look different than the one in the example. This is normal. The major concern you should have is that your *program* outputs exactly the displayed format.

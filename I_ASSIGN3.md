# Programming Assignment #2 (Full Adder)

In this assignment you will write an application that simulates a chain of full adders to illustrate how two numbers rae added at the logical level.

## Specifications

Your program MUST adhere to these specifications:

* You can expect that your program will receive exactly two valid positive integers in the range allowed by a 32-bit signed integer. You do *not* need to error-check the user's input.

## Required Input and Output

* When run, the program must ask for the first number and wait for the user to input the number.
* After receiving the first number, the program should ask for the second number.
* Once it has both numbers, it should perform the computations to display the state of all five "pins" on each full adder in a chain long enough to add the two numbers.

    For each adder, you'll indicate the value of the two input bits (A and B), the carry-in bit (CI), the carry-out bit (CO), and the sum bit (S).

    For your program, you do not need to print a fancy table, but *you may if you wish*. 
* Finally, at the end of the program, print the result both in binary and in decimal.

When printing the output of your adders, *each line* must include, in this order: the A and B bits, the sum bit, the carry-in and carry-out bits, and the current "running sum".

Here is an example of valid output for your program:

```
enter number: 13
enter number: 9

1 1 0 0 1 0
0 0 1 1 0 10
1 0 0 1 0 110
1 1 0 0 1 0110
0 0 1 0 1 10110

answer: 10110
number: 22
```

Note that your program does not need to conform *exactly* to this format. However, you do need to present the elements of the adders in the order given.

Use either `0` and `1` or `T` (true) and `F` (false) to indicate your bits. You can choose either, but be consistent.

# Scoring Rubric

| Item | Points | Notes |
|-|-|-|
| Functional program | 75 | Program produces correct values for output and uses the specified algorithms and techniques. Point loss varies based on incorrect implementation |
| Output format is as required | 25 | The program must print the adder chain in the specified format. |

This assignment is worth 100 points, with 15 extra credit points possible.


# Floating Point and IEEE 754 Assignment

For this assignment, unless otherwise noted, use *half-precision* (16-bit) floating point numbers.

## Problem 1

Convert the following decimal numbers into IEEE 754 half-precision format. 

> [!IMPORTANT]
> Show *all work*, including:
> 
> * Integer/fraction separation
> * Binary conversion steps
> * Normalization
> * Exponent handling (hint: bias the exponent)
> * Final sign, exponent, mantissa fields
>
> You do not need to show conversion between binary and hexadecimal.

1. 5.25
2. 0.5
3. -13.25
4. 214.875

Finally, convert the number `276.25` to a **single precision (32-bit)** floating point value. Write the result in both binary and hexadecimal.

> [!TIP]
> See the section listing [defined floating point formats](FP.md#defined-floating-point-number-formats) from the reference guide to learn how many bits you need to use for the mantissa and exponent.

## Problem 2

1. Convert the number `0.2` (decimal) into IEEE 754 half-precision format. Normalize, bias, and encode. Write the final bit pattern in **hexadecimal**.

    At what point (after how many rounds of multiplication) can you determine the **pattern** that emerges? In other words, at what point can you tell for sure that you will end up in an infinite loop? What tells you this fact?

2. Convert 5.25 to half-precision, but store it as if the bias were **16** instead of 15. What does this cause to happen? Why?

## Problem 3

The following half-precision numbers are stored in memory as the given value. Decode them back to decimal.

1. 0 10000 1010000000
1. 0100 0101 1101 0000
1. `0x590B`

For each:

- Extract sign, exponent, fraction.
- Write the exact rational value as a fraction. For example: `181/256`
- Express the decimal to 6 places.

## Problem 4

The half-precision representation of 0.7 is: `0x399A`. 

- Decode this back into decimal *exactly*
- Compute the absolute error compared to `0.7`. (Subtract the value you receive from `0.7`)

    Is the stored value larger or smaller than the real value?

- If you were to add this quantity 10,000 times per second for 15 seconds, what would the **cumulative drift** be?

## Problem 5

Answer briefly:

1. Why does 0.5 have an exact binary representation, but 0.1 does not?
2. Between which two half-precision numbers does `0.2` lie? Why does IEEE choose the one it does?
3. Suppose we increased the mantissa to 23 bits (single precision). Would 0.1 be exact then? Why or why not?
4. A student claims `0x4200` represents 3.0 in half-precision. Without actually converting the number to decimal, explain why this must be wrong.
5. Consider what would happen if we were to change the number of bits for exponent and mantissa. If we were to instead use only 4 bits for the exponent and use 11 bits for the mantissa, would this *increase* or *decrease* precision? Why?

# Scoring Rubric

| Problem | Points | Notes |
|-|-|-|
| 1 | 20 | 4 points per conversion problem.<br />2 points for correct answer, 2 points for process correctness |
| 2 | 20 | 10 points per question. |
| 3 | 15 | 5 points per decode. 2 points for correct answer, 3 points for process correctness |
| 4 | 15 | 5 points per question. |
| 5 | 30 | 6 points per question. |

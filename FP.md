# Floating Point Conversion Reference

## To convert *into IEEE floating point* from decimal:

1. Convert the integer portion (to the *left* of the decimal point) to binary. This will become the *leftmost* portion of the mantissa.
2. Convert the *decimal portion* of the number into binary by using *[successive multiplication](#successive-multiplication)*. Repeat the successive multiplication process until you either 1) end up with a 0, or 2) run out of bits in the mantissa.

    If the floating point section does not use up all the mantissa bits, fill out the bits with 0's to the right.
3. Determine where the first "1" in the mantissa is with relation to the current position of the decimal point.

    In IEEE 754, a leading "1" is *implied*. So, once you find the proper location of the decimal point, you can *ignore the leading 1* and store only the fractional part of the number (after exponentiation) in the mantissa.
4. Convert the exponent into binary.
5. *Bias* the exponent. The bias is the number represented by all `1`'s, with the length being one less than the size of the exponent field.

    For half-precision floating point, five bits are used for the exponent. Thus, the bias is the value that four 1's, `1111`, has in binary. This value is `15`. 

    To bias the exponent, just add this number to the exponent.
6. If the original number was positive, set the sign bit (leftmost bit) to 0; if it was negative, set it to 1.
7. Concatenate the bits for the sign bit, exponent, and mantissa.

## To convert *from IEEE floating point* back to decimal:

1. Handle the exponent. First, *un-bias* the exponent (same method: subtract the bias value) and convert the binary number to an integer. Then, with the decimal point starting between the first two digits of the mantissa: for positive exponents, move the decimal point to the *right*; for negative exponents, move it to the *left*.

    If you are dealing with large exponents or negative exponents, and your decimal point would go outside the bounds of the mantissa, imply 0's in the extra space. For example, if you start with `1.01010000` and your exponent is `-4`, you would shift the decimal point to the left 4 times. Since you'll move off the left end of the mantissa, you would fill in with zeros: `0.00010101`.
2. To handle the fractional component, add *successive reciprocals of powers of 2*. For the first digit following the decimal point, the value would be 0.5 (`1/2`); for the second digit, it would be 0.25 (`1/4`), and so on.
3. If there is an integer section, convert it to an integer like usual.
4. Combine the integer and decimal section.
5. If the sign bit of the original number was 1, flip the sign - i.e. make the number negative.

## Successive Multiplication

To convert fractional components into binary, you use the *successive multiplication* method.

> [!TIP]
> If you're unfamiliar with the *successive division* method for converting integeres to binary, refer to its [section in this document](#successive-division-refresher)

To perform successive multiplication:

1. Remove any integer component from your number. Example: "5.5" should become `0.5`.
2. Multiply the fractional component by 2. 
3. If the result is **1 or greater**, add a `1` to the *right-hand side* of your fractional binary number. If the result is still less than 0, add a `0`.
4. If the result is 1 or greater, subtract 1 - in other words, remove the integer component again.
5. **Repeat** steps 2 through 4 until either:
    - the result of multiplying and then removing the integer yields 0, or
    - you run out of bits in the mantissa to represent more fractional digits.

### Example

Convert the decimal quantity `0.703125` into binary.

1. Multiply 0.703125 by 2: `1.40625` This is greater than 1, so we write a `1`.

    *Current output number*: `1`

    We also subtract 1 from our accumulator to remove the integer: `0.40625`
2. Multiply 0.40625 by 2: `0.8125`. This is less than 1, so we write a `0`.

    *Current output number*: `10`
3. Multiply 0.8125 by 2: `1.625`. This is greater than 1, so we write a `1`.

    *Current output number*: `101`

    We also subtract 1 from the accumulator: `0.625`
4. Multiply 0.625 by 2: `1.25`. This is greater than 1, so we write a `1`.

    *Current output number*: `1011`

    Subtract 1: `0.25`
5. Multiply 0.25 by 2: `0.5`. This is less than 1, so write a 0.

    *Current output number*: `10110`
6. Multiply 0.5 by 2: `1.0`. This is exactly 1, so we write a 1.

    *Current output number*: `101101`

    Subtract 1 from the accumulator: `0`.
7. Our accumulator is `0`, so we are finished!

If there are more digits in the mantissa, we simply fill them out with 0's. For a 16-bit half-precision number, we need 10 bits of mantissa, and we can imply the leading 1. Therefore, our final number - the value we would store in the mantissa - would be `0110100000`.

## Converting Fractional Component from Binary to Decimal

Converting the fractional component of a binary number back to decimal can seem daunting. However, there is a shortcut you can use to make the process simpler.

Let's use the example number `.10110101`.

The "straightforward" approach for this is to start working from the decimal point, and add increasing *reciprocals* of powers of 2. In this number, we'd add `1/2`, `1/8`, `1/16`, `1/64` and `1/256`.

If your calculator can add fractions, this might be fine for you. But if you're doing more of this by hand and would like to reduce the number of calculations, you can *normalize the fractions to the highest common denominator*.

To figure out the highest common denominator, it's easy - just calculate 2<sup>n</sup>, where n is the number of bits in the value. (To keep your life simple, only count up to the last 1 bit - ignore trailing 0's.) In this case, we have 8 digits, so our HCD is `256`.

Then, we can *convert the fractional component into decimal* ***as if it were an integer***! The result of converting `10110101` to an ingeter is `181`.

Then, we simply make this result the numerator, and the denominator is the HCD. We get `181/256`. We could then compute this on a calculator to get the decimal version of the fractional component!

# Complete Example - Convert from Decimal to Floating Point

Convert the number `5.625` into a half-precision IEEE 754 floating point number.

1. Is the number positive or negative? It is positive. So we start with a `0` in the sign bit.
2. Convert the integer section into binary. `5` in binary is `101`.
3. Convert the fractional component, independent of the integer. So, we'll convert `0.625` into decimal. The result of this conversion is also `101`. (See the above [Successive Multiplication](#successive-multiplication) section to learn how this is done.)
4. Concatenate the two numbers, keeping the decimal point: `101.101`.
5. What do we need to do to the decimal point to make this number into the form `1.x`? We need to shift the decimal point to the **left**, **two** places. 

    Recall that shifting to the left *adds* to the exponent. Therefore, our exponent is `2`.
6. Bias the exponent. In half-precision, we know that we need to add `15` to the exponent. Therefore, our exponent is `17`. In binary, this is `10001`.

    If your exponent is less than the number of bits needed after biasing, pad the result with `0` to the *left*. For example, if you got `100` after biasing, you would store `00100`.
7. Since the leading `1` is implied, we truncate it from the mantissa. Then, we remove our decimal point to get the mantissa value: `01101`.

    This number is less than the 10 bits required for the mantissa, so we pad the number to the *right*: `0110100000`
8. Concatenate the sign bit, the exponent bits, and the mantissa bits: `0 10001 0110100000`
9. The final result: `0100 0101 1010 0000`
10. We can represent the result in hexadecimal by converting each individual block of 4 bits into an integer. We use the letters A through F for the values 10 through 15. For this number, the result is `45A0`

# Complete Example - Convert from Floating Point to Decimal

Convert the IEEE 754 half-precision float `1101011100111000` into decimal.

1. It's easier if you split the digits up into their components. For half-precision, 1 bit is the sign bit, 5 bits are the exponent, and 10 bits are the mantissa.

    Sign bit: `1`
    Exponent: `10101`
    Mantissa: `1100111000`
2. Reverse-bias the exponent - *subtract* the bias value. In this case, the bias is `1111` (15). `10101 - 1111` = `110`. Thus, our exponent is `6`.
3. Prepend a `1.` to the mantissa to make the conversion simpler. Our mantissa now looks like this: `1.1100111000`.
4. **Shift** the decimal point the required number of places. Shift to the right for positive exponents, to the left for negative exponents.

    For this number, the result becomes `1110011.100`.

    If the shift pushes the decimal point off of either end of the mantissa, implied `0`s are added to fill in the gaps. This doesn't apply to this number, but assume the exponent was 15. The resulting number would look like: `1111001110000000`. The five `0`s on the end are implied, since our decimal point "fell off" the end of the mantissa.
5. Convert the *integer portion* of the number back into decimal. The number for our example is `1110011`. The result of this conversion is `115`. Thus, we know that the *integer* portion of our number is 115.
6. Convert the *fractional* component. For us, we have only three digits left to represent the fractional part: `100`. This one is simple enough that we can "eyeball" it - a fraction with a `1` at the left and all `0`'s to the right is `0.5` in decimal.

    If we had a more complex fractional part, we would use the method described in [Converting Fractional Component from Binary to Decimal].(#converting-fractional-component-from-binary-to-decimal)
7. Now we simply combine the integer and fractional component: `115.5`
8. Finally, since the sign bit is a 1, we simply flip the sign of the number.

Our final answer: `-115.5`.

# Why can't we represent 0.1 in IEEE floating point?

Let's try to convert 0.1 into a half-precision float!

1. The integer portion is 0. So our integer section is just a 0.
2. Let's work the [successive multiplication](#successive-multiplication) algorithm to figure out what our fractional component should be:

    * `0.1 * 2` = `0.2`. Result: `0`
    * `0.2 * 2` = `0.4`. Result: `00`
    * `0.4 * 2` = `0.8`. Result: `000`
    * `0.8 * 2` = `1.6`. Result: `0001`. Subtract 1.
    * `0.6 * 2` = `1.2`. Result: `00011`. Subtract 1.
    * `0.2 * 2` = `0.4`. Result: `000110`.
    * `0.4 * 2` = `0.8`. Result: `0001100`
    * `0.8 * 2` = `1.6`. Result: `00011001`. Subtract 1.
    * `0.6 * 2` = `1.2`. Result: `000110011`. Subtract 1.
    * `0.2 * 2` = `0.4`. Result: `0001100110`.

    Note that we now have 10 mantissa digits, so we've run out of room. *More importantly, we've entered into an infinite loop* - the accumulator will *never reach 0*!

    We can also see the repeating pattern. To write `0.1` decimal in binary, we'd need to write an *infinite sequence* of `00110011001100110011...`. 

    Let's go ahead and continue with our conversion. 
3. Since we know this pattern now, and we know that the mantissa has an implied 1, let's figure out our exponent - how do we move the decimal point such that it is after the first 1 of our number?

    `0.0001100110011...`

    We need to move the decimal point *four places to the right* to get `00001.100110011...` Therefore, our exponent is `-4`.
4. We add the bias of 15 to -4, so our exponent is `11`. Converted to binary, this is `1011`. Since the exponent is five bits, we pad to the left: `01011`.
5. We now fill out as many mantissa bits as we can: `1001100110`. 
6. This becomes the *best we can do*: `0 01011 1001100110`.

    In hex: `0010 1110 0110 0110` -> `2E66`

Let's close the loop and reverse the conversion - this will show us the *error* compared to the correct value of `0.1` decimal.

1. Unbias the exponent - `01011 - 1111 = 11100` when we allow for "loop-around" on underflow. In twos-component representation, this is the value `-4`.
2. Add a `1` to the beginning of the mantissa, then perform the decimal point shift - a negative exponent means to move to the *left*.

    `1.1001100110`

    Move the decimal point to the left, adding zeroes as appropriate:

    `0.00011001100110`

    > We imply the 0 to the left of the decimal point, just as we would in decimal - `.5` is the same as `0.5`.
3. The integer portion of our number is 0, so we don't have to convert it.
4. After the shift, we actually have **14 bits** of mantissa to work with. We can use the method described in [Converting Fractional Component from Binary to Decimal](#converting-fractional-component-from-binary-to-decimal) to convert these bits as a fractional component.

    2<sup>14</sup> = 16384.

    We can ignore the leading 0's and convert the value `11001100110` into decimal. This becomes `1638`.

    Then we calculate `1638 / 16384`. If you put this into Windows Calculator, the result given is: `0.0999755859375`.

There's a few things to note here:

* The result we got is *very close to 0.1*, but not *precisely 0.1*! 
* When you look at the fraction, you see "1638 / 16384" - notice that they both start with `1638`? 

    If we wanted to get the exact result of `0.1`, we'd need to actually divide `1638.4 / 16384`.

    However, doing this means we'd need to represent `0.4` in binary somehow - which has the exact same problem as representing `0.1`!

> [!TIP]
> Remember that **only numbers that can be expressed as perfect fractions where the denominator is a power of 2** can be precisely represented in IEEE 754 floating point!
>
> We accept this trade-off because, when working with larger floating point numbers, the *precision* is great enough that we can account for it when we do calculations. (However, as we saw in class, failing to consider this imprecise representation can result in interesting - sometimes devastating - bugs!)
>
> In practice, computers can still do floating point math because they will intelligently *round* the results of calculations. If you add `0.1` and `0.2` in a typical computer calculator, you'll get the correct result: `0.3`. *However*, neither `0.2` nor `0.1` can be precisely represented in binary! This works because the computer will use rounding internally to maintain the precision of your calculations.
>
> Thus, even though adding "0.1" and "0.1" might actually be adding "0.099996" and "0.099996" internally, the result - "0.199992" will be *rounded up* to one decimal place - this is how you get `0.2` as the result.

# Defined floating point number formats

| Name | Total Bit Size | Exponent Bits | Mantissa Bits | Bias Value |
|-|-|-|-|-|
| Half-precision | 16 | 5 | 10 | 15 (2<sup>4</sup> - 1) |
| Single precision | 32 | 8 | 23 | 127 (2<sup>7</sup> - 1) |
| Double precision | 64 | 11 | 52 | 1023 |
| Quad precision | 128 | 15 | 112 | 16383
| Octuple (8x) precision | 256 | 19 | 236 | 262143 |
| Binary-512 | 512 | 25 | 486 | 16777215 |
| FP8 (for AI) | 8 | 4 | 3 | 7 |

# Special Cases

IEEE 754 defines the following special cases:

* All `0` bits across the entire value: `0`.
* All `0` bits, with a `1` in the sign bit: `negative 0`.
* Exponent bits are all `1`, with the mantissa set to `0`: Infinity (or negative infinity if sign bit is set)
* Exponent bits are all `1`, with the mantissa as any nonzero value: Not a Number (`NaN`). Can be used as the result of invalid calculations (such as dividing by zero) or complex numbers (square root of `-1`).
* Exponent bits are all `0` and the mantissa is nonzero: This is a special case known as *subnormal numbers*.

## Subnormal numbers

Subnormal numbers are a special case for conversion. If the exponent bits are all `0` in your number, you compute the decimal version as follows:

1. Add an implied leading `0.` to the mantissa as written. 
2. The exponent is 1 minus the bias value. For half precision, this becomes 2<sup>-14</sup>.
3. The conversion from that point is the same as usual, *without implying the leading `1`*.

# Successive Division Refresher

This section serves as a quick refresher on how to convert integers from decimal into binary.

Let's convert the number `1234` to binary.

We divide `1234` by `2` and determine both the quotient and the remainder (also known as the *modulus*). We store the quotient and we write the modulus down to the *left* of any existing digits. This process repeats until the quotient and remainder are both 0.

* `1234 / 2 = 617` with no remainder. Output value: `0`
* `617 / 2 = 308.5`. This means we have a remainder of 1. Output value: `10`. (Note how we write the 1 to the *left* of the existing value!) We also drop the fraction from the quotient - our new value is 308.
* `308 / 2 = 154`. No remainder. Output value: `010`
* `154 / 2 = 77`. No remainder. Output value: `0010`
* `77 / 2 = 38.5`. Remainder 1. Output value: `10010`. 
* `38 / 2 = 19`. No remainder. Output value: `010010`
* `19 / 2 = 9.5`. Output value: `1010010`
* `9 / 2 = 4.5`. Output value: `11010010`
* `4 / 2 = 2`. Output value: `011010010`
* `2 / 2 = 1`. Output value: `0011010010`
* `1 / 2 = 0.5`. Output value: `10011010010`.
* We've reached `0`, so we're finished.

The answer is `10011010010`.
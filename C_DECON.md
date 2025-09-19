# Deconstructing a simple C program

## The problem

> Write a simple program that will read a number from the user. The number should be printed out with its digits reversed. You must use the **division and modulus** method to reverse the digits.
>
> You can assume the input will be a valid integer that fits into a 32-bit signed integer, and that the number will be positive. You can also assume that the flipped digits will still fit into a 32-bit integer. You do not need to check for invalid input.
>
> Your program should print `Enter a number: ` and then wait for the user's input. You should then reverse the digits, and display `Result: ` followed by the result.

## The solution

```
#include <stdio.h>

int main() {

    int num;
    int reversed = 0;

    printf("Enter a number: ");

    scanf("%d", &num);

    while (num != 0) {
        int digit = num % 10;
        reversed = reversed * 10 + digit;
        num /= 10;
    }

    printf("Result: %d\n", reversed);

    return 0;

}
```

## Deconstructing the program

What follows is a line-by-line explanation of the program

`#include <stdio.h>`

This line tells the C preprocessor to include the contents of the file `stdio.h` in the program code at the location of the `#include` statement. The `#include` statement will be replaced with the contents of the file.

The purpose of including `stdio.h` is to get access to functions that work with input and output (**st**andar**d** **i**nput and **o**utput). In particular, we need the `scanf` function, which reads input, and the `printf` function, which prints or writes output.

`int main(void) {`

This line starts our C program's `main` function. 

> [!TIP]
> Remember: All C programs *must* contain exactly one `main` function, and that function must follow specific signatures. The *main* function is the function that is run when your program starts.

In this `main` function, we are not accepting any parameters (`()`), so we will not be able to read arguments passed on the command line. That is OK for this program.

The function's return type is `int`, therefore this program will be able to return a value to the program that launched it.

`int num;`

This line initializes a variable called `num`, but does not set a value.

Depending on the C implementation, the variable will be initialized to a default value, or in some older implementations, the space is allocated but nothing is done - so you could have ended up with a random arbitrary value depending on what might have been left over in RAM.

`int reversed = 0;`

This line initializes another int, called `reversed`, and explicitly sets its initial value to `0`.

`printf("Enter a number: ");`

The `printf` function writes some data to the program's *standard output stream*. When you run a program at the command line, the standard output stream is connected to your console. 

In this case, we print the exact string `Enter a number: `. (Note the space at the end of the string!)

`scanf("%d", &num);`

The `scanf` function reads one line of input from the *standard input stream*. Like standard output, standard input is connected to your keyboard when you run the program at the command line.

The string being passed to `scanf` has a special meaning. It consists of a **format code** - in this case `%d`. Format codes define what *format* the data being read is expected to take on. In this instance, we're telling `scanf` that the input received should be considered to be an integer.

The magic is that `scanf` takes care of converting the value to an integer for us - we don't have to do the messy calculation from ASCII digit characters to an actual binary value stored in memory.

The second parameter to `scanf` is unique - it's the `num` variable, but it is preceded by an ampersand. The ampersand `&` has a special meaning when it prefixes a variable: it tells the program that the *numeric memory address* of where the variable indicated is stored in memory should be passed, as an actual numeric value.

This is important because, in classic C, *all* variables are passed by value - that is, a copy of the value is made in memory and the address of that copy is provided to the function. Most notably, this means that you can't pass a *reference* to some value (like a string) to a function normally - you'd be passing a *copy* of the string, and if the function changed the string, those changes would be lost as soon as the function ends (they would not propagate upwards to the original variable).

The `&` character allows us to implement a "by reference" variable. What we're doing here is giving `scanf` the *memory address* at which the variable `num` is stored. The actual address is arbitrary and depends on many factors, but you can always assume the value of `&num` is the numeric memory address where the value of `num` is kept.

With these two parameters, we're telling `scanf`: 

- read a line of input from standard input
- that line should consist of exactly one integer
- take the string that represents an integer and convert it to a binary integer
- store that integer at the *memory location* at which `num` is stored

The key takeaway here:

> C does not pass parameters into functions by reference. It makes a copy of that value and provides that to the function.
>
> You can "override" this behavior by passing memory addresses rather than actual values into functions. A copy of a memory address is still the same number and will still same point to the same actual memory. 

`while (num != 0) {`

This line starts a `while` loop. Like most languages, a Boolean condition is given inside the parentheses. The `while` loop will run until the expxression evaluates to false. The expression is evaluated at the *start* of each loop; therefore, if the expression already evaluates to false, the loop will not run at all.

`int digit = num % 10;`

This is the first step of the divide-and-modulus method of reversing digits.

Here, we extract the *rightmost* digit from the number, by doing a modulo 10. This will always give us whatever the rightmost digit is in a base-10 number.

`reversed = reversed * 10 + digit;`

Multiplying the `reversed` value by 10 has the effect of *shifting all the digits one position to the left*. We then add on the digit we just plucked off the right side of the original value.

`num /= 10;`

In C, integer division ignores the remainder. This will have the effect of shifting the entire original number to the right by one place - and letting the rightmost digit that we extracted earlier "fall off" the number (since we are ignoring remainders and decimals).

`printf("Result: %d\n", reversed);`

This line prints the result.

`printf` uses the same format codes as `scanf`. 

In this case, we're telling `printf` that we want to print the string `Result: `, and then we want to include the *string representation* of an integer (again, the `%d`).

The `\n` is a newline character. Including it in your string causes the cursor to move to the next line and to the first column - just like pressing Enter.

> [!TIP]
> Python automatically adds a `\n` character to the end of strings when using the `print` statement. In Python, this can be overriden by adding an `end=` parameter - for example, `print("The value is: ",end="").

The second parameter to `printf` is the number to print.

> [!TIP]
> Both `printf` and `scanf` can accept multiple format codes. To use more codes, you simply provide more parameters to the function, in the order the codes appear in the string.
>
> Example:
>
>     printf("Hi %s! You're %d years old and your pet's name is %s!\n", &szName, age, &szPetName)

`return 0;`

This final statement ends the program.

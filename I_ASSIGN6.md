# Final Assignment: Building a Data Encoding Library

## Overview

In this assignment, you will create a **C library** that provides a function for decoding data in a simple binary format. You will then write a **separate program** that uses your library to process some test data.

This assignment ties together many concepts from the course:

- Binary representation and bitwise operations
- C programming with structs
- The compilation pipeline (compiling, linking, libraries)

---

## The Protocol

Your library will implement decoding for a simple **message format** used to transmit sensor readings. Each message is a sequence of bytes with the following structure:

```
Message Structure (8 bytes total):
┌─────────────────────────────────────────────────────────┐
│ Byte 0:     Message Type (1 byte)                       │
│ Byte 1:     Flags (1 byte)                              │
│ Bytes 2-3:  Sensor ID (2 bytes, little-endian uint16)   │
│ Bytes 4-7:  Reading (4 bytes, little-endian int32)      │
└─────────────────────────────────────────────────────────┘
```

### Field Descriptions

**Message Type** (1 byte):
- `0x01` = Temperature reading (value in tenths of degrees Celsius)
- `0x02` = Humidity reading (value in tenths of percent)
- `0x03` = Pressure reading (value in Pascals)

**Flags** (1 byte) — individual bits have meaning:
- Bit 0 (0x01): Alert flag — reading is outside normal range
- Bit 1 (0x02): Low battery flag — sensor battery is low
- Bits 2-7: Reserved (ignore these)

**Sensor ID** (2 bytes):
- Unsigned 16-bit integer identifying which sensor sent the reading
- Stored in **little-endian** byte order

**Reading** (4 bytes):
- Signed 32-bit integer containing the sensor value
- Stored in **little-endian** byte order
- Interpretation depends on message type

---

## Requirements

You will create the following files:

### 1. Header File

Create a file called `protocol.h`. Your header file must contain:

1. **A struct definition** for the decoded message:
   ```c
   typedef struct {
       // You define the fields
   } SensorMessage;
   ```

2. **A function prototype** for:
   - `int decode_message(const uint8_t *data, SensorMessage *msg);`

3. **Constant definitions** for message types (using `#define` or `enum`)

    > [!TIP]
    > Remember that `#define` works just as a simple find/replace in the preprocessor. Therefore, you could do:
    >
    >     #define FIRST_VALUE 1
    >     #define SECOND_VALUE 2
    >     int result = FIRST_VALUE + SECOND_VALUE;
    >     printf("Result: %d\n", result);
    >
    > Keep in mind that `int result = FIRST_VALUE + SECOND_VALUE;` would **not** be valid C code if you had not defined variables named `FIRST_VALUE` and `SECOND_VALUE`. However, the preprocessor will **replace** all instances of `FIRST_VALUE` in the code with `1`, and all instances of `SECOND_VALUE` with `2`, *before* the C compiler itself even sees the code.
    >
    > Recall that preprocessor constants are useful for fixed values that might change at *compile time*, but *never change* at runtime.

### 2. Implementation File

Create a file `protocol.c` and implement one function:

**`int decode_message(const uint8_t *data, SensorMessage *msg)`**
- Takes a pointer to 8 bytes of raw data
- Parses the bytes and fills in the `SensorMessage` struct
- Returns `0` on success, `-1` on error (e.g., invalid message type)

> [!IMPORTANT]
> **Important implementation notes:**
> - You must ensure proper handling of little-endian byte order for multi-byte fields. All data will be in little-endian format.
> - You must use bitwise operations to extract/set flag bits. 
> - Do NOT use any non-standard library functions.

> **Assumption"**
>
> You may assume the data will be *valid* - you do not need to worry about buffer overflows or arrays that are too small being passed to the function. (This means that for testing, *you need to provide valid data!*)

### 3. Test Program

Write a program that:

1. Creates at least three byte representations of `SensorMessage` in `uint8_t` arrays (byte arrays)

    > Hint: You can put arbitrary hexadecimal values into a byte array directly, like this:
    >
    >     uint8_t testMessage[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

2. Decodes the encoded bytes to structs using your `decode_message()` function. Create an empty `SensorMessage` struct to pass to the function, and remember to use pointer syntax when passing the struct!

3. Prints the results from the struct using dot notation and standard `printf` commands.

Print the data for each struct in one of these formats as appropriate:

    Temperature, sensor 7, alert clear, battery OK, 23.5 C
    Humidity, sensor 42, alert set, battery OK, 95.2%
    Pressure, sensor 256, alert set, battery low, 101325 Pa

> [!IMPORTANT]
> We did not throughly cover **casting** in class. Since all of the values come as integers, but you're expected to treat them as tenths when printing, you'll need to **cast** the integer value you derive from the data into a **float**.
>
> You can do that explicitly like this:
>
>     float floatValue = (float)intValue;
>
> This of course assumes `intValue` is an integer and already contains the value you intend to convert.
>
> Casting lets you tell the runtime that you want to do the conversion; the actual steps to convert the number are handled for you behind the scenes.
 
> [!TIP]
>
> - You should always print the alert and battery flags even if they're unset. For alert, use `set` and `clear` for `1` and `0` respectively; for battery, use `OK` and `low` for `0` and `1`, respectively.
> - You can use two percents `%%` to put a percent sign inside of a `printf` format string.
> - You'll want to use an if/else if statement or a switch statement to print the correct type of output - temperature should be `x.y C`, humidity should be `x.y%`, and pressure should be `x Pa` (`x` is any number before the decimal point and `y` is any number after it; both temperature and humidity are expressed in tenths, but pressure is in literal Pa units.)

Your test data must include at least:
- One message of each type (temperature, humidity, pressure)
- At least one message with multiple flags set
- At least one message with a negative reading value

You must do a bare minimum of three test messages, but more is always good! Below are several examples you can use for testing, but *you must also provide three of your* ***own*** *messages with your* ***own*** *sample values!*

### 4. Building the program

Follow these steps to build your program:

1. First, compile your *library*. If you named your library `protocol.c`, then run:

        gcc -O2 -c protocol.c -o protocol.o

    Recall that this will produce the compiled but *not yet linked* object file.

2. Create a static archive of your compiled object file.

        ar rcs libprotocol.a protocol.o
    
    This creates the `.a` file that the linker can use later to produce a complete executable binary.

3. Build the main program, linking statically:

        gcc -O2 --static main.c -L. -lprotocol -o mcs276_a6

    Recall that the linker will look for `.a` (archive) files when building code statically. Essentially, the `.a` file is analogous to a ZIP file containing a copy of the `.o` file. (`.a` files are not limited to just one object file - large libraries, like the C standard library, bring many `.o` files together into a single `.a` file.)

    You can now execute the program:

        ./mcs276_a6

**If you make changes:**

If you have changed *only the `main.c` file* - i.e. you have NOT made any changes to the library, you need only follow step 3 to recompile the `main.c` file. Since the library is already compiled, you can skip the step of compiling it again. (This is one of the main advantages of building C programs as collections of `.c` library files rather than as one large monolithic `.c` source file - it minimizes the need to recompile code based on small changes!)

If you *have* changed the `protocol.c` file, then you will need to perform *all three steps above* to rebuild the application. 

* If you only rebuild the library but do not rebuild the application, the application will still use the old library because that is what was used when the program was compiled.
* If you only rebuild the application, changes in the library won't take effect since they must be recompiled *and relinked*.

## Example Test Data

Here are some example messages you can use for testing. Your program should be able to decode decode these 8-byte messages into structs correctly.

**Example 1: Temperature Reading**

    // Temperature reading, sensor ID 1, no flags, 23.5 degrees C.
    uint8_t example1[] = { 0x01, 0x00, 0x01, 0x00, 0xEB, 0x00, 0x00, 0x00 };

**Example 2: Humidity with Alert**

    // Humidity reading, sensor ID 42, alert flag set, reading 95.2%
    uint8_t example2[] = { 0x02, 0x01, 0x2A, 0x00, 0xB8, 0x03, 0x00, 0x00 };

**Example 3: Pressure with Multiple Flags**

    // Pressure reading, sensor ID 256, 101.3 kPa (101325 Pa), low battery alert
    uint8_t example3[] = { 0x03, 0x02, 0x00, 0x01, 0xCD, 0x8B, 0x01, 0x00 }

**Example 4: Negative Temperature**

    // Temperature reading, sensor ID 7, alert set and low battery, negative 15 degrees C
    uint8_t example4[] = { 0x01, 0x03, 0x07, 0x00, 0x6A, 0xFF, 0xFF, 0xFF }

---

## Submission Requirements

Submit the following files:

1. `protocol.h` — Your header file
2. `protocol.c` — Your implementation file
3. `main.c` — Your test program

You do NOT need to submit a compiled copy of your program. **Remember you must submit your** ***source code*** - submitting only a binary will result in no points (since I can't read your code!)

---

## Grading Rubric

| Component | Points | Details |
|-----------|--------|---------|
| **Header File** | 20 | Correct struct, prototypes, constants |
| **encode_message()** | 50 | Correctly decodes all fields, handles endianness and flags |
| **Test Program** | 30 | Tests all message types per requirements |

**Total: 100 points**

---

## Hints and Tips

1. **Endianness:** Remember that little-endian stores the least significant byte first. For a 16-bit value `0x0102`, the bytes in memory are `02 01`.

2. **Bitwise operations for flags:**
   - To specify the bit position of a flag: `FLAG_ALERT = 1 << n` (replace `n` with the bit position, least significant first. The first bit from the right would simply be `FLAG_ALERT = 1 << 0`; the next bit (second bit from the right) might be `SOME_ALERT = 1 << 1`...)
   - To check if a flag is set: `if (flags & FLAG_ALERT) { ... }`
   - To set a flag: `flags |= FLAG_ALERT;`
   - To clear a flag: `flags &= ~FLAG_ALERT;`

3. **Accessing individual bytes:** Use `uint8_t` (from `<stdint.h>`) for byte-level access.

4. **Building multi-byte integers from bytes:**
   ```c
   uint16_t value = data[0] | (data[1] << 8);  // Little-endian
   ```

5. **Extracting bytes from integers:**
   ```c
   data[0] = value & 0xFF;         // Low byte
   data[1] = (value >> 8) & 0xFF;  // High byte
   ```

6. **Negative numbers:** Recall that two's complement representation means negative numbers have their high bits set. When you extract the bytes of `-150` as a 32-bit int, you'll see `FF FF` in the high bytes.

7. **Include `<stdint.h>`** for `uint8_t`, `uint16_t`, `int32_t`, etc.

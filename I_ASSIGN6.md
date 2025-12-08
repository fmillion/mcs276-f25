# THIS IS NOT COMPLETE. THIS IS A WORK IN PROGRESS THAT IS CHANGING IN REAL TIME. DO NOT START THIS ASSIGNMENT UNTIL THIS MESSAGE GOES AWAY!

# Assignment 6: Building a Data Encoding Library

## Overview

In this assignment, you will create a **C library** that provides functions for decoding data in a simple binary format. You will then write a **separate program** that uses your library to process some test data.

This assignment ties together many concepts from the course:

- Binary representation and bitwise operations
- C programming with structs and pointers
- The compilation pipeline (compiling, linking, libraries)
- Understanding memory layout
- Using GDB to examine program state

## The Protocol

Your library will implement encoding and decoding for a simple **message format** used to transmit sensor readings. Each message is a sequence of bytes with the following structure:

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

### 1. `protocol.h` — Header File

Your header file must contain:

1. **A struct definition** for the decoded message:
   ```c
   typedef struct {
       // You define the fields
   } SensorMessage;
   ```

2. **Function prototypes** for:
   - `int decode_message(const uint8_t *data, SensorMessage *msg);`

3. **Constant definitions** for message types (using `#define` or `enum`)

4. **Proper include guards**

### 2. `protocol.c` — Implementation File

Implement the function:

**`int decode_message(const uint8_t *data, SensorMessage *msg)`**
- Takes a pointer to 8 bytes of raw data
- Parses the bytes and fills in the `SensorMessage` struct
- Returns `0` on success, `-1` on error (e.g., invalid message type)

**Important implementation notes:**
- You must handle little-endian byte order correctly for multi-byte fields
- You must use bitwise operations to extract/set flag bits
- Do NOT use any non-standard library functions

### 3. `main.c` — Test Program

Write a program that:

1. Creates several `SensorMessage` structs with test data (hardcoded)
2. Encodes each message using your `encode_message()` function
3. Decodes the encoded bytes using your `decode_message()` function
4. Prints the results to verify round-trip encoding/decoding works

Your test program must include at least:
- One message of each type (temperature, humidity, pressure)
- At least one message with multiple flags set
- At least one message with a negative reading value

### 4. `Makefile` — Build Configuration

Use the provided Makefile template (see below). You must fill in the parts marked with `TODO`.

---

## Provided Makefile Template

```makefile
# Makefile for Final Assignment
# MCS 276 - Intro to Systems 1

CC = gcc
CFLAGS = -Wall -g

# Library name
LIBNAME = protocol

# TODO: Fill in your source file name
MAIN_SOURCE = <YOUR_MAIN_FILE_HERE>.c

# =============================================================================
# STATIC LIBRARY BUILD
# =============================================================================

# Compile protocol.c to object file
$(LIBNAME).o: $(LIBNAME).c $(LIBNAME).h
	$(CC) $(CFLAGS) -c $(LIBNAME).c -o $(LIBNAME).o

# Create static library
lib$(LIBNAME).a: $(LIBNAME).o
	ar rcs lib$(LIBNAME).a $(LIBNAME).o

# Build main program with static linking
static: $(MAIN_SOURCE) lib$(LIBNAME).a
	$(CC) $(CFLAGS) $(MAIN_SOURCE) -L. -l$(LIBNAME) -o program_static

# =============================================================================
# SHARED LIBRARY BUILD
# =============================================================================

# Compile protocol.c with -fPIC for shared library
$(LIBNAME)_shared.o: $(LIBNAME).c $(LIBNAME).h
	$(CC) $(CFLAGS) -fPIC -c $(LIBNAME).c -o $(LIBNAME)_shared.o

# Create shared library
lib$(LIBNAME).so: $(LIBNAME)_shared.o
	$(CC) -shared -o lib$(LIBNAME).so $(LIBNAME)_shared.o

# Build main program with shared linking
shared: $(MAIN_SOURCE) lib$(LIBNAME).so
	$(CC) $(CFLAGS) $(MAIN_SOURCE) -L. -l$(LIBNAME) -Wl,-rpath,. -o program_shared

# =============================================================================
# UTILITY TARGETS
# =============================================================================

all: static shared

clean:
	rm -f *.o *.a *.so program_static program_shared

.PHONY: static shared all clean
```

## Expected Output Format

Your test program should print output similar to this:

```
=== Testing Sensor Protocol Library ===

--- Test 1: Temperature Reading ---
Original: type=1 flags=0x00 sensor=1 reading=235
Encoded bytes: 01 00 01 00 eb 00 00 00
Decoded: type=1 flags=0x00 sensor=1 reading=235
Round-trip: PASS

--- Test 2: Humidity with Alert ---
Original: type=2 flags=0x01 sensor=42 reading=952
Encoded bytes: 02 01 2a 00 b8 03 00 00
Decoded: type=2 flags=0x01 sensor=42 reading=952
Round-trip: PASS

[... more tests ...]

=== All tests completed ===
```

---

## Submission Requirements

Submit the following files:

1. `protocol.h` — Your header file
2. `protocol.c` — Your implementation file
3. `main.c` — Your test program

---

## Grading Rubric

| Component | Points | Details |
|-----------|--------|---------|
| **Header File** | 15 | Correct struct, prototypes, include guards, constants |
| **decode_message()** | 25 | Correctly parses all fields, handles endianness and flags |
| **encode_message()** | 25 | Correctly encodes all fields, handles endianness and flags |
| **Test Program** | 15 | Tests all message types, flags, negative values; clear output |
| **Makefile** | 10 | Both static and shared targets work correctly |
| **GDB Evidence** | 10 | Shows breakpoint, struct contents, and raw bytes |

**Total: 100 points**

---

## Hints and Tips

1. **Endianness:** Remember that little-endian stores the least significant byte first. For a 16-bit value `0x0102`, the bytes in memory are `02 01`.

2. **Bitwise operations for flags:**
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

6. **Negative numbers:** Two's complement representation means negative numbers have their high bits set. When you extract the bytes of `-150` as a 32-bit int, you'll see `FF FF` in the high bytes.

7. **Include `<stdint.h>`** for `uint8_t`, `uint16_t`, `int32_t`, etc.

---

## Academic Integrity

This is an individual assignment. You may discuss concepts with classmates, but all code must be your own. Do not share code or copy from online sources.

---

*Good luck! This assignment brings together many concepts from the semester. Start early and test incrementally.*

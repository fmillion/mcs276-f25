# GDB Quick Reference

This reference covers the essential GDB commands you'll need for this course. GDB is a powerful debugger, but you only need a small subset of its capabilities to be effective.

---

## Getting Started

### Compiling for Debugging
Always compile with the `-g` flag to include debug symbols:
```bash
gcc -g -o myprogram myprogram.c
```

Without `-g`, GDB will still work, but it won't be able to show you source code lines or variable names.

### Launching GDB
```bash
gdb ./myprogram           # Start GDB with your program
gdb ./myprogram core      # Start GDB with a core dump file
```

### Exiting GDB
```
(gdb) quit                # or just 'q'
```

---

## Running Your Program

| Command | Short | Description |
|---------|-------|-------------|
| `run` | `r` | Start (or restart) program execution |
| `run arg1 arg2` | | Run with command-line arguments |
| `continue` | `c` | Resume execution after a breakpoint |
| `kill` | | Stop the running program |

---

## Breakpoints

Breakpoints pause execution at a specific location.

| Command | Description |
|---------|-------------|
| `break main` | Break at the start of `main()` |
| `break function_name` | Break at the start of a function |
| `break filename.c:42` | Break at line 42 of filename.c |
| `break 42` | Break at line 42 of current file |
| `info breakpoints` | List all breakpoints |
| `delete 1` | Delete breakpoint number 1 |
| `delete` | Delete all breakpoints |
| `disable 1` | Temporarily disable breakpoint 1 |
| `enable 1` | Re-enable breakpoint 1 |

---

## Stepping Through Code

| Command | Short | Description |
|---------|-------|-------------|
| `step` | `s` | Execute one line, **enter** function calls |
| `next` | `n` | Execute one line, **skip over** function calls |
| `finish` | | Run until current function returns |
| `until 50` | | Run until line 50 |

**Tip:** Press Enter with no command to repeat the last command. This is very useful when stepping through code!

---

## Examining Variables

| Command | Description |
|---------|-------------|
| `print variable` | Print the value of a variable |
| `print *pointer` | Print what a pointer points to |
| `print array[3]` | Print element 3 of an array |
| `print &variable` | Print the address of a variable |
| `print/x variable` | Print in hexadecimal |
| `print/t variable` | Print in binary |
| `print/c variable` | Print as a character |
| `info locals` | Print all local variables |
| `info args` | Print function arguments |

**Tip:** `p` is shorthand for `print`.

---

## Examining Memory

The `x` (examine) command lets you look at raw memory contents.

**Syntax:** `x/NFU address`
- **N** = Number of units to display
- **F** = Format: `x` (hex), `d` (decimal), `c` (char), `s` (string), `i` (instruction)
- **U** = Unit size: `b` (byte), `h` (halfword/2 bytes), `w` (word/4 bytes), `g` (giant/8 bytes)

### Common Examples:
```
(gdb) x/16xb &variable      # 16 bytes in hex starting at variable's address
(gdb) x/4xw &variable       # 4 words (32-bit each) in hex
(gdb) x/s str_pointer       # Memory as a null-terminated string
(gdb) x/10i $rip            # Next 10 instructions (x86-64)
(gdb) x/10i $pc             # Next 10 instructions (ARM64)
```

### Useful tasks with these operations:
- Seeing how multi-byte integers are stored (endianness!)
- Examining string contents in memory
- Looking at the stack contents

---

## Examining the Stack

| Command | Description |
|---------|-------------|
| `backtrace` | Show the call stack (which functions called which) |
| `bt` | Short for `backtrace` |
| `frame N` | Switch to stack frame N |
| `up` | Move up one stack frame (toward `main`) |
| `down` | Move down one stack frame |
| `info frame` | Detailed information about current frame |

**What the backtrace shows you:**
```
(gdb) backtrace
#0  crash_function (ptr=0x0) at demo.c:5
#1  0x0000555555555185 in helper (x=42) at demo.c:10
#2  0x00005555555551a2 in main () at demo.c:15
```
- Frame #0 is where you currently are
- Each frame shows the function name, arguments, and location
- You can switch frames to examine local variables at each level

---

## Examining Registers

| Command | Description |
|---------|-------------|
| `info registers` | Show all general-purpose registers |
| `info all-registers` | Show ALL registers (including floating point) |
| `print $rax` | Print a specific register (x86-64) |
| `print $x0` | Print a specific register (ARM64) |

### Key Registers (x86-64 / WSL on Windows):
| Register | Purpose |
|----------|---------|
| `rax` | Return value, general purpose |
| `rdi`, `rsi`, `rdx`, `rcx`, `r8`, `r9` | Function arguments (in order) |
| `rsp` | Stack pointer |
| `rbp` | Base pointer (frame pointer) |
| `rip` | Instruction pointer (program counter) |

### Key Registers (ARM64 / Mac):
| Register | Purpose |
|----------|---------|
| `x0` | Return value, first argument |
| `x0`-`x7` | Function arguments (in order) |
| `sp` | Stack pointer |
| `x29` (fp) | Frame pointer |
| `pc` | Program counter |

---

## When Things Crash

When your program segfaults under GDB, it will stop and give you control. Use these commands to investigate:

1. **Where did it crash?**
   ```
   (gdb) backtrace
   ```

2. **What were you trying to do?**
   ```
   (gdb) list                 # Show source code around crash
   (gdb) info registers       # What's in the CPU?
   ```

3. **What's the suspicious pointer?**
   ```
   (gdb) print pointer_variable
   (gdb) print *pointer_variable    # This will fail if pointer is bad
   ```

4. **What's at that memory location?**
   ```
   (gdb) x/16xb bad_address
   ```

---

## Useful Tips

### Repeat Commands
Just press **Enter** to repeat the last command. Very useful when stepping!

### Command History
Use the **up/down arrow keys** to navigate through previous commands.

### Text User Interface (TUI)
```
(gdb) layout src          # Show source code in a split view
(gdb) layout asm          # Show assembly code
(gdb) layout regs         # Show registers
(gdb) tui disable         # Turn off TUI mode
```

### Pretty Printing
```
(gdb) set print pretty on     # Format structs nicely
(gdb) set print array on      # Format arrays nicely
```

### Getting Help
```
(gdb) help                    # General help
(gdb) help breakpoints        # Help on breakpoint commands
(gdb) help x                  # Help on the examine command
```

---

## Quick Reference Card

| Action | Command |
|--------|---------|
| Start debugging | `gdb ./program` |
| Run program | `r` |
| Set breakpoint | `b function` or `b file:line` |
| Continue | `c` |
| Step into | `s` |
| Step over | `n` |
| Print variable | `p variable` |
| Print in hex | `p/x variable` |
| View memory | `x/16xb &var` |
| Call stack | `bt` |
| See registers | `info registers` |
| Local variables | `info locals` |
| Quit | `q` |

---

## Common Mistakes to Avoid

1. **Forgetting `-g` when compiling**
   - GDB will work but can't show source code
   
2. **Using `step` when you meant `next`**
   - `step` enters functions; `next` skips over them
   
3. **Not setting a breakpoint before `run`**
   - Program will run to completion (or crash) without stopping
   
4. **Confusing `print` and `x`**
   - `print` interprets the variable according to its type
   - `x` examines raw memory bytes

5. **Forgetting that optimized code behaves differently**
   - If you compile with `-O2`, variables may be "optimized out"
   - For debugging, compile with `-g -O0` (no optimization)

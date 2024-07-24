<h1 align='center'>libasm</h1>

*All the explanations in this file follow the System V AMD64 ABI calling convention.*

### Apple Silicon

Apple Silicon is based on ARM64 architecture. The assembly code in this repository is written for `x86_64` architecture.

If you wish to use and test your assembly code seamlessly on those chips, you need to add the following lines to your Makefile:

```makefile
CC = gcc
ifeq ($(shell uname -m), arm64)
	CC += -ld_classic --target=x86_64-apple-darwin
endif
```

Instead of rewriting all the assembly code for ARM64 architecture, we basically compile our C code in `x86_64` architecture as it can be executed through Rosetta 2.

> Don't forget the `-f macho64` flag after your `nasm` command when compiling on Macs.

## Prerequisites and basic concepts

### Registers

Assembly works mainly with registers. They can be compared to little pre-defined boxes that can store data.

The general-purpose registers are:
- `rax`
- `rcx`
- `rbx`
- `rdx`
- `rsi`
- `rdi`
- `rsp`
- `rbp`
- `r8`, `r9`, ...`r15`
- `rip`

We have to be careful when putting data in those registers, because they can be used by the system at any time (read or written).

Among the above registers for example:
- `rax` is used to store the return value of a function.
- `rcx` is used as a counter in loops.
- `rsp` is used to store the stack pointer.
- `rbp` is used to store the base pointer.
- `rdi`, `rsi`, `rdx`, `rcx`, `r8`, `r9` are used respectively to pass arguments, vulgarly like `function(rdi, rsi, rdx, rcx, r8, r9)`

> This may seem like an inconvenience, but it is actually very useful to manipulate the behavior of the program.
>
> For instance, if we want to call `sys_write`:
> 1. We put the syscall number (4 for `sys_write`) in `eax`.
> 2. We put the file descriptor in `ebx`.
> 3. We put the address of the buffer in `ecx`.
> 4. We put the number of bytes to write in `edx`.
> 5. We call the `syscall` instruction.

The conclusion is: **the little boxes are very useful as intermediate storage for data, but we have to be careful not to overwrite them when we** (or the system) **need them.**

### Execution flow

Assembly is read **from top to bottom**.

The instructions can be grouped in **labels**, which are used to **mark a specific point in the code**. They are followed by a colon.

One thing that can be confusing is that although labels look like functions in other languages like C, they are not.

For example:
```nasm
entry_point:
	xor rax, rax

do_something:
	mov rax, 0

do_something_else:
	mov rax, 1

return_label:
	ret
```

In this example, `entry_point` is the entry point of the program/function.

`do_something` and `do_something_else` will be executed one after the other, even without a "jump" instruction.

### Syntax

> The syntax used in this repository is the Intel syntax. It is the most common syntax used in assembly programming, and a requisite of the subject. It is characterized by the fact that the destination operand is on the left and the source operand is on the right.

#### Instructions

Virtually **all the lines in assembly are** composed of an **instruction followed by its operand(s)**.

A few examples:


- `mov rax, 0` copies the value `0` into the register `rax`.

- `add rax, 1` adds `1` to the value in the register `rax`.

- `cmp rax, 0` compares the value in the register `rax` with `0`.

- `jmp do_something` jumps to the label `do_something`.

#### Behavior

It is very important to remember that every instruction can alter the behavior of the program implicitly.

For example:

- The `cmp` instruction will set the flags register according to the result of the comparison.
- The `loop` instruction will decrement the `rcx` register and jump to the label if `rcx` is not zero.

#### Addresses and values

Like in C, we can work with addresses.

The square brackets `[]` are used to dereference an address.

For example, if we want to move the value at the address `0x1234` into the register `rax`, we can do:

```nasm
mov rax, [0x1234]
```

If we want to compare the address 3 bytes after the address in `rax` with `0`, we can do:

```nasm
cmp [rax + 3], 0
```

> Here we should technically use an identifier for the address (`BYTE`, `WORD`, `DWORD`, `QWORD`) to specify the size of the data we want to compare, but we ignored it for the sake of this explanation.

#### Exporting symbols

In order to use the functions we write in assembly in a C program, we need to export them.

To do so, we can use the `global` directive.

For example, if we want to export the function `ft_strlen`, we can do:

```nasm
global ft_strlen

ft_strlen:
	...
```

## ft_strlen

The `ft_strlen` function is a function that returns the length of a string. It is a very simple function that iterates over the string until it finds the null-terminator (`\0`).

To implement it in assembly, we need to recapitulate the behavior of the function:
1. Set a counter to 0.
2. Look at the first character of the string.
3. Increment the counter.
4. Look at the next character.
5. If it is not the null-terminator, increment the counter and go back to step 4.
6. If it is the null-terminator, return the counter.

To replicate this behavior in assembly, we will need to learn a few instructions:
- `mov` to copy data.
- `jmp` to jump.
- `cmp` to compare data.
- `je` to jump if equal.
- `inc` to increment a register.
- `ret` to return from the function.

#### `mov`

```nasm
mov rax, rdi
```

This instruction copies the value in `rdi` to `rax`.

#### `jmp`

```nasm
entry_point:
	jmp some_other_label

some_label:
	...

some_other_label:
	...
```

The first line of `entry_point` will jump to the label `some_other_label` (and skip `some_label`) regardless of the condition.

Jumps work like a `goto` in C. They can be used to skip parts of the code, or to create loops (as they can jump to a label that is located earlier in the code).

#### `cmp`

```nasm
cmp rax, rdi
```

This instruction compares the value in `rax` with the value in `rdi`.

If we want to check if `rax` is equal to `rdi`, we can do:

```nasm
cmp rax, rdi
je equal
```

Which leads us to the next instruction.

#### `je`

```nasm
cmp some_register, some_other_register
je equal
```

This instruction will jump to the label `equal` only if the two registers are equal.

#### `inc`

```nasm
inc rax
```

This instruction increments the value in `rax` by `1`. Simple.

#### `ret`

```nasm
ret
```

This instruction returns from the program/function.

> Remember that in the System V AMD64 ABI, the return value of a function is stored in the `rax` register. Whatever is in `rax` when we call `ret` instruction will be the return value of the program/function.

### Implementation

Now that we know the instructions we need, we can implement the `ft_strlen` function.

The first thing we need to do is to **set the counter to 0**. We can do this by moving `0` to `rcx` (or any other register, but remember `rcx` is commonly used as a counter).

```nasm
mov rcx, 0
```

Then, we need to define our recurring loop.

We need to look at every character in the string passed in `rdi` (where is passed the first argument in this calling convention, as seen before).

So, **`rdi` first points at the first character of the string**, and **`rcx` is our counter initialized to 0**.

Like we would look into `*(str + i)` in C, we can use assembly's square brackets `[]` as follows:

```nasm
cmp [rdi + rcx], 0
```

What happens next?
- If the character is **not the null-terminator**, we need to **increment the counter and go back** to the beginning of the loop.
- If **it is the null-terminator**, we need to **return the counter**.

So, with the instructions we learned before, we can use:
- `cmp` to compare the character with `0`
- `je` to jump to the end of the function if it is the null-terminator.
- `inc` to increment the counter.
- `jmp` to go back to the beginning of the loop.

Our loop would then look like:

```nasm
loop:
	cmp [rdi + rcx], 0
	je end
	inc rcx
	jmp loop
```

Finally, we need to return the counter. We can do this by moving the value in `rcx` to `rax` and returning.

> Remember, that *"whatever is in `rax` when we call `ret` instruction will be the return value of the program/function."*

```nasm
end:
	mov rax, rcx
	ret
```

And that's it! We have implemented the `ft_strlen` function in assembly.

## ft_strcpy

The `ft_strcpy` function is a function that copies a string into another string, and returns a pointer to the destination string.

The logic is very similar to the `ft_strlen` function:

1. Set a counter to 0.
2. Look at the first character of the source string.
3. Copy it to the destination string.
4. Increment the counter.
5. Look at the next character and copy it.
6. If it is not the null-terminator, increment the counter and go back to step 5.
7. If it is the null-terminator, exit the loop and return a pointer to the destination string.

### Implementation

A pointer to our `dst` string is passed in `rdi`, and a pointer to our `src` string is passed in `rsi`.

We can start the same way we did with `ft_strlen` by setting the counter to 0.

```nasm
ft_strcpy:
	mov rcx, 0
```

We can then start our loop.

We need to copy every character in `rsi` to `rdi`. However, **in assembly, we can't copy data directly from one address to another** (`mov [rdi], [rsi]` would not work).

We therefore need to copy the data from the source address to a register, and then copy it to the destination address.

> We could use any register to store the character (like `r8` as seen before), but it is more appropriate to use `al` for this purpose, as it is a register that is meant to store a single byte.

```nasm
loop:
	mov al, [rsi + rcx]
	mov [rdi + rcx], al
	inc rcx
	cmp al, 0
	jne loop
```

In this loop:
1. We copy the character in `rsi` to `al`.
2. We copy `al` to `rdi`.
3. We increment the counter.
4. We check if the character is the null-terminator.
5. If it is not, we go back to the beginning of the loop.

Finally, we need to return the pointer to the destination string.

Given that we received this pointer in `rdi` and that we did not move it, we can simply copy it to `rax` and return.

```nasm
mov rax, rdi
ret
```

## ft_strcmp

The `ft_strcmp` function is a function that compares two strings, and returns the difference between the first two different characters. For example, "abc" and "abd" would return -1, as 'c' - 'd' = -1.

Its logic is **not much more complex** than the previous functions, but **the particularities of assembly make it a bit more challenging** (for me at least, maybe I have a shitty logic).

Let's recap the behavior of the function anyway:
1. Set a counter to 0.
2. Compare every character of the first string with every character of the second string.
3. If they are different, substract the second character from the first character and return the result.

From this exercise on, I will only explain the new instructions and concepts we use.

#### `sub`

```sub rax, rdi``` performs a substraction such as `rax` ← `rax` - `rdi`.

#### `movzx`

```movzx rax, BYTE[rdi + rcx]``` moves the byte at the address `rdi + rcx` to `rax`, and fills the remaining bits with 0.

> movzx will adapt to the keyword used to specify the size of the data we want to move. For example, `movzx rax, WORD[rdi + rcx]` would move a word (16 bits) to `rax`.

This instruction is useful to us as `rax` is a 64-bit register, and we only want to compare the characters as bytes (8 bits).

#### `jz`

```jz label``` jumps to `label` if the zero flag is set, a bit like `je` and `jne` but for the zero flag.

For example, if we want to jump to `end` if one of `register1` or `register2` is 0, we can do:

```nasm
cmp register1, 0
cmp register2, 0
jz end
```

### Implementation

In this implementation, **I will not show code like in the previous ones**, just describe the logic with more depth.

My intermediates registers will be `rax` and `r8` (not the most efficient code but easier to understand).

1. We set `rcx`, `rax` and `r8` to 0.
2. We start our loop.
3. We copy `rdi` and `rsi` to `rax` and `r8`.
4. We check that the characters are not the null-terminator.
5. We compare the characters.
6. If they are different, we substract them and return.
7. If not, we increment the counter and go back to the beginning of the loop.

That's it! With all the precautions I mentioned and the new instructions, you should be able to implement the `ft_strcmp` function.

## ft_write

The `ft_write` function is a function that, provided a file descriptor, a buffer and a size, writes the buffer to the file descriptor. It returns the number of bytes written, or -1 if an error occurred.

To implement this function, we need to know how to make a syscall.

### Syscalls

As we saw in the introduction, to make a syscall, we need to:
1. Put the syscall number in `rax`.
2. Put its arguments in the appropriate registers (here `rdi`, `rsi` and `rdx`).
3. Trigger the syscall with the `syscall` instruction, that will read the values in the registers and behave accordingly.

### First steps

In our case, the parameters we receive from C are already in the right registers, so we can save the following instructions:
```nasm
mov rdi, rdi ; file descriptor
mov rsi, rsi ; buffer
mov rdx, rdx ; count
```

We can then put the syscall number in `rax` and call the syscall.

On Apple Silicon Macs, the syscall number for `sys_write` is `0x2000004`. On Linux, it is `1`.

```nasm
mov rax, 0x2000004
; mov rdi, rdi
; mov rsi, rsi
; mov rdx, rdx
syscall
ret
```

We could stop here, as the function would properly write to the file descriptor and return the number of bytes written (the syscall putting its return value in `rax`).

However, **we need to return `-1` if an error occurred, and set the `errno` variable accordingly**, as asked in the subject.

To handle any error and jump to another label, we can use the `jc` instruction. This will **jump to the label if the Carry Flag is set**, which is generally the case when an error occurs.

```nasm
jc error
```

#### `errno`

`errno` is a variable that is set when an error occurs. It is a global variable that is set by the system when a syscall fails.

However, **it is not automatically translated to the `errno` variable in C**. We need to set it ourselves.

To do so, we are provided with `__errno_location` (or `___error` on Mac). This function returns a pointer to the `errno` variable.

We can call it with the instruction `call __errno_location`. As for other instructions, it will put the return value in `rax`.

However, **`rax` already contains the return value of the `write` syscall**. We need to **save it** before calling `__errno_location`.

```nasm
error:
	mov r8, rax
	call __errno_location
```

We now have the address of the `errno` variable in `rax`. We can put the previously saved error code in it by dereferencing the address.

```nasm
mov [rax], r8
```

Finally, we can return `-1` and exit the function.

```nasm
mov rax, -1
ret
```

And that's it! We have implemented the `ft_write` function.

# Resources

- [x64 Cheat Sheet](https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf)
- [Le langage assembleur intel 64 bits](https://www.lacl.fr/tan/asm)
- [___error on Mac vs __errno_location on Linux](https://github.com/cacharle/libasm_test/issues/2)

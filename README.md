<h1 align='center'>libasm</h1>

*All the explanations in this file follow the System V AMD64 ABI calling convention.*
*It is important to note that it will only work on a x86_64 architecture, therefore not on Apple Silicon (ARM64) for example.*

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
> 1. We put the syscall number (1 for `sys_write`) in `rax`.
> 2. We put the file descriptor in `rdi`.
> 3. We put the address of the buffer in `rsi`.
> 4. We put the number of bytes to write in `rdx`.
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

To do so, we can use the `.global` directive.

For example, if we want to export the function `ft_strlen`, we can do:

```nasm
.global ft_strlen

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

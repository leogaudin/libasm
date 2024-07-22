*All the explanations in this file follow the System V AMD64 ABI calling convention.*

## Registers

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

The conclusion is: **the little boxes are very useful as intermediate storage for data, but we have to be careful not to overwrite them when we need them.**

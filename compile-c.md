
# Compiling to C

## General Code Layout

### runtime.h

Every compiled program depends on a runtime. The runtime is expected to present
the following interface in `runtime.h`. This runtime is expected to provide the
IO interface.

``` cpp
#ifndef RUNTIME_H
#define RUNTIME_H

typedef /* Appropriate integer type */ int_type;

void print(int_type n);
void cprint(int_type n);

int_type read();
int_type cread();

void dbg(int_type stack[], int_type size);

#endif // RUNTIME_H

```

### Main program

The main program contains 5 variables; `size`, `stack`, `x`, `y`, and `addr`.
`size` stores the current size of the stack. `stack` is the actual stack itself,
with the maximum stack size being determined at compile time. `x` and `y` are
temporary variables. Lastly `addr` stores the address of the instruction that a
jump instruction is targeting. Since TSTK jumps can consume any number as input,
a special lookup table must be kept matching the instruction number to the
appropriate C instruction. As such, labels must be kept identifying each TSTK
instruction with the appropriate series of C instructions. These instructions
have the form `L0`, `L1`, etc. When a jump occurs the TSTK instruction number is
stored in `addr` and execution branches to the `lookup` label.

``` cpp
#include "runtime.h"

int main(int argc, char const *argv[]) {
  int_type size = 0;
  int_type stack[/* stack size */];

  int_type x;
  int_type y;
  int_type addr = 0;

  lookup:
  switch(addr) {
    case 0: goto L0;
    // Rest of the lookup table...
  }

  L0:

  // Compiled code...

  return 0;
}
```

## Compilation Table

The following is are individual compilation mappings for each TSTK instruction.

Any integer `n`:
``` cpp
stack[size] = n;
size++;
```

add, sub, mul, div:
``` cpp
size--;
x = stack[size];
size--;
y = stack[size];
stack[size] = y /* +, -, *, or / */ x;
size++;
```

dup:
``` cpp
stack[size] = stack[size - 1];
size++;
```

jmp:
``` cpp
size--;
addr = stack[size];
goto lookup;
```

jeq, jnq, jgt, jlt:
``` cpp
size--;
x = stack[size];
size--;
y = stack[size];
size--;

if(y /* ==, !=, >, or < */ x) {
  addr = stack[size];
  goto lookup;
}
```

get:
``` cpp
size--;
x = stack[size];
stack[size] = stack[(size - 1) - x];
size++;
```

set:
``` cpp
size--;
x = stack[size];
size--;
y = stack[size];
stack[(size - 1) - y] = x;
size++;
```

pop:
``` cpp
size--;
```

ppos: The instruction address `n` of ppos should be known at compile time, so
all we have to do is push that address.

``` cpp
stack[size] = n;
size++;
```

print, cprint:
``` cpp
size--;
x = stack[size];
print(x);  // Replace with cprint as necessary.
```

read, cread:
``` cpp
stack[size] = read();  // Replace with cread as necessary.
size++;
```

size:
``` cpp
stack[size] = size;
size++;
```

swap:
``` cpp
x = stack[size - 2];
stack[size - 2] = stack[size - 1];
stack[size - 1] = x;
```

dbg:
``` cpp
dbg(stack, size);
```

## Optimizations

### Jump Optimization

For normal jumps we have a complexity time of `O(n)`, where `n` is the size of
the program. This is because every must lookup its jump location in a lookup
table, since TSTK jumps take as an argument the instruction to jump to, which
may not be known at compile time. However, if the jump instruction is
immediately following an integer or label reference, such as in `7 jmp` or
`@lp jlt`, then we do know the jump location at compile time. So instead of
`7 jmp` being compiled to

``` cpp
Ln:
stack[size] = 7;
size++;

Lm:
size--;
addr = stack[size];
goto lookup;
```

it can instead be compiled to

``` cpp
Ln:
goto L7;

Lm:
size--;
addr = stack[size];
goto lookup;
```

Note that when we go to the nth command, `7 jmp` is guaranteed to be the same as
simply going to the 7th instruction. However, if we go to the mth instruction,
then we are simply executing `jmp` and have no information about the jump
location. We must therefore do a lookup. One example of how this could happen
is something like `99 m jmp`, in which case we would be jumping to instruction
99 instead of instruction 7 at `Lm`.

Additionally `7 jeq` would compile to

``` cpp
Ln:
size--;
x = stack[size];
size--;
y = stack[size];

if(y == x) {
  goto L7;
}

Lm:
size--;
x = stack[size];
size--;
y = stack[size];
size--;

if(y == x) {
  addr = stack[size];
  goto lookup;
}
```

The code for `jnq`, `jlt`, and `jgt` are all similar to `jeq` (just replace the
`==`).

This optimization can turn the must common jump instruction, a jump to an
address, from a `O(n)` operation into an `O(1)` operation, greatly improving
performance.

## Important Things to Remember

* Every compiled instruction has a label before it in the form L*n* where *n* is
the instruction address it was compiled from.

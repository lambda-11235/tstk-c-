
# Compiling to C

* Every compiled instruction has a label before it in the form L*n* where *n* is
the line it was compiled from.

* The lookup table matches the address in `addr` and goes to the appropriate label.

## General Code Layout

### Runtime.h

``` c++
#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdlib.h>

size_t* new_stack(size_t capacity);
void push(size_t* stack, size_t& size, size_t& capacity, size_t elem);
size_t pop(size_t* stack, size_t& size, size_t& capacity);

#endif // RUNTIME_H

```

### Main program

``` c++
#include <stdlib.h>

#include "runtime.h"

int main(int argc, char const *argv[]) {
  size_t addr = 0;
  size_t size = 0;
  size_t capacity = 2;
  size_t* stack = new_stack(capacity);

  int x;
  int y;
  int addr;

  lookup:
  switch(addr) {
    case 0: goto L0;
    // Rest of the lookup table...
  }

  L0:

  // Compiled code...

  free(stack);

  return 0;
}
```

## Compilation Table

Any integer n:
``` c++
push(stack, size, capacity, n);
```

add:
``` c++
x = pop(stack, size, capacity);
y = pop(stack, size, capacity);
push(stack, size, capacity, y + x);
```

sub:
``` c++
x = pop(stack, size, capacity);
y = pop(stack, size, capacity);
push(stack, size, capacity, y - x);
```

* Add more commands from README.md.

jmp:
``` c++
addr = pop(stack, size, capacity);
goto lookup;
```

* Add more commands from README.md.

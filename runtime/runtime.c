
#include <stdio.h>

#include "runtime.h"


void print(int_type n) {
  printf("%lld\n", n);
}

void cprint(int_type n) {
  printf("%c", (char) n);
}

int_type read() {
  int_type n;
  scanf("%lld", &n);
  return n;
}

int_type cread() {
  return getchar();
}

void dbg(int_type stack[], int_type size) {
  printf("[");

  for(int i = 0; i < size; i++) {
    printf("%lld", stack[i]);

    if(i != size - 1)
      printf(", ");
  }
}

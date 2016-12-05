// Programmer: Taran Lynn
// Programmer's ID: 1513043

#include <stdexcept>

#ifndef EXCEPTION_STACK_H
#define EXCEPTION_STACK_H

class StackUnderflow : std::exception {
public:
  StackUnderflow() {}

  const char* what() const throw() {
    return "Stack underflow";
  }
};

#endif /* end of include guard: EXCEPTION_STACK_H */

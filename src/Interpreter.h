
#include <iostream>
#include <string>
#include <vector>

#include "ast.h"


#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter {
  std::vector<Instruction> instructions;
  std::vector<int_type> stack;
  int_type ip;  // The instruction pointer. It contains the index of the
                // currently executing instruction.

public:
  Interpreter();

  /**
   * Adds the instructions in a set of tokens to the currently executing program.
   */
  void addTokens(std::vector<Token> toks);

  /**
   * Resets the program, but does not remove any instructions.
   */
  void reset();

  /**
   * Runs the instructions, stopping after the instruction pointer reaches the
   * last instruction.
   */
  void run();

  /**
   * Print the current stack to STDOUT.
   */
  void printStack();
};

#endif /* INTERPRETER_H */

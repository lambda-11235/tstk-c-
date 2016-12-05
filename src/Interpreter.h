// Programmer: Taran Lynn
// Programmer's ID: 1513043

#include <iostream>
#include <string>
#include <vector>

#include "ast.h"
#include "Stack.h"


#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter {
  std::vector<Instruction> instructions;

  // The tokens corresponding to each instruction. Note that all labels are
  // removed from the token list.
  std::vector<Token> tokens;

  Stack<int_type> stack;

  // The instruction pointer. It contains the index of the currently executing
  // instruction.
  int_type ip;

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
   * Runs a single instruction.
   */
  void runInstruction();

  /**
   * Runs the instructions, stopping after the instruction pointer reaches the
   * last instruction.
   */
  void run();

  /**
   * Print the current stack to STDOUT.
   */
  void printStack();

  /**
   * Determines if the interpreter still has instructions to run.
   */
  inline bool stillRunning() const { return ip < instructions.size(); }

  /**
   * Returns the next instruction to be executed.
   */
  inline Instruction currentInstruction() const { return instructions[ip]; }

  /**
   * Returns token of the next instruction to be executed.
   * Useful for getting debugging information.
   */
  inline Token currentToken() const { return tokens[ip]; }
};

#endif /* INTERPRETER_H */


#include <iostream>
#include <string>
#include <vector>

#include "ast.h"


#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter {
  std::vector<Instruction> instructions;
  std::vector<int_type> stack;
  int_type ip;

public:
  Interpreter();

  void addTokens(std::vector<Token> toks);
  void reset();
  void run();

private:
  void printStack();
};

#endif /* INTERPRETER_H */

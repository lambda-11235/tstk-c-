
#include <iostream>
#include <string>
#include <vector>

#include "ast.h"
#include "Interpreter.h"


Interpreter::Interpreter() {
  ip = 0;
}


void Interpreter::addTokens(std::vector<Token> toks) {
  std::vector<Instruction> insts = removeLabels(toks, instructions.size());

  // Avoid extra copying of elements.
  instructions.reserve(instructions.size() + insts.size());
  tokens.reserve(tokens.size() + insts.size());

  for(Instruction inst : insts) {
    instructions.push_back(inst);
  }

  for(Token tok : toks) {
    if(tok.type != LABEL)
      tokens.push_back(tok);
  }
}


void Interpreter::reset() {
  stack.clear();
  ip = 0;
}


void Interpreter::run() {
  while(stillRunning()) {
    runInstruction();
  }
}


void Interpreter::runInstruction() {
  Instruction inst = instructions[ip];
  int_type x;
  int_type y;
  int_type n;


  if(inst.pushInt) {
    stack.push(inst.inst.n);
  } else {
    // NOTE: Arguments are popped off of the stack in reverse order.
    switch(inst.inst.com) {
      case ADD:
        y = stack.pop();
        x = stack.pop();

        stack.push(x + y);
        break;

      case SUB:
        y = stack.pop();
        x = stack.pop();

        stack.push(x - y);
        break;

      case MUL:
        y = stack.pop();
        x = stack.pop();

        stack.push(x * y);
        break;

      case DIV:
        y = stack.pop();
        x = stack.pop();

        stack.push(x / y);
        break;

      case JMP:
        ip = stack.pop() - 1;
        break;

      case JEQ:
        n = stack.pop();
        y = stack.pop();
        x = stack.pop();

        if(x == y) {
          ip = n - 1;
        }

        break;

      case JNQ:
        n = stack.pop();
        y = stack.pop();
        x = stack.pop();

        if(x != y) {
          ip = n - 1;
        }

        break;

      case JGT:
        n = stack.pop();
        y = stack.pop();
        x = stack.pop();

        if(x > y) {
          ip = n - 1;
        }

        break;

      case JLT:
        n = stack.pop();
        y = stack.pop();
        x = stack.pop();

        if(x < y) {
          ip = n - 1;
        }

        break;

      case GET:
        n = stack.pop();

        stack.push(stack.get(n));
        break;

      case SET:
        n = stack.pop();
        x = stack.pop();

        stack.set(n, x);
        break;

      case DUP:
        stack.push(stack.top());
        break;

      case POP:
        stack.pop();
        break;

      case PPOS:
        stack.push(ip);
        break;

      case SIZE:
        stack.push(stack.size());
        break;

      case SWAP:
        y = stack.pop();
        x = stack.pop();

        stack.push(y);
        stack.push(x);
        break;

      case DBG:
        printStack();
        break;

      case PRINT:
        std::cout << stack.pop() << std::endl;
        break;

      case CPRINT:
        std::cout << (char) stack.pop();
        break;

      case READ:
        std::cin >> x;
        stack.push(x);
        std::cin.ignore();
        break;

      case CREAD:
        stack.push(std::cin.get());
        std::cin.ignore();
        break;
    }
  }

  ip++;
}


void Interpreter::printStack() {
  std::cout << stack << std::endl;
}

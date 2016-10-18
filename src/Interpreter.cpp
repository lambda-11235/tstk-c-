
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

  for(Instruction inst : insts) {
    instructions.push_back(inst);
  }
}


void Interpreter::reset() {
  stack.clear();
  ip = 0;
}


void Interpreter::run() {
  while(ip < instructions.size()) {
    Instruction inst = instructions[ip];
    size_t x;
    size_t y;
    size_t n;


    if(inst.pushInt) {
      stack.push_back(inst.inst.n);
    } else {
      // NOTE: Arguments are popped off of the stack in reverse order.
      switch(inst.inst.com) {
        case ADD:
          y = stack.back();
          stack.pop_back();

          x = stack.back();
          stack.pop_back();

          stack.push_back(x + y);
          break;

        case SUB:
          y = stack.back();
          stack.pop_back();

          x = stack.back();
          stack.pop_back();

          stack.push_back(x - y);
          break;

        case MUL:
          y = stack.back();
          stack.pop_back();

          x = stack.back();
          stack.pop_back();

          stack.push_back(x * y);
          break;

        case DIV:
          y = stack.back();
          stack.pop_back();

          x = stack.back();
          stack.pop_back();

          stack.push_back(x / y);
          break;

        case JMP:
          ip = stack.back() - 1;
          stack.pop_back();
          break;

        case JEQ:
          n = stack.back();
          stack.pop_back();

          y = stack.back();
          stack.pop_back();

          x = stack.back();
          stack.pop_back();

          if(x == y) {
            ip = n - 1;
          }

          break;

        case JNQ:
          n = stack.back();
          stack.pop_back();

          y = stack.back();
          stack.pop_back();

          x = stack.back();
          stack.pop_back();

          if(x != y) {
            ip = n - 1;
          }

          break;

        case JGT:
          n = stack.back();
          stack.pop_back();

          y = stack.back();
          stack.pop_back();

          x = stack.back();
          stack.pop_back();

          if(x > y) {
            ip = n - 1;
          }

          break;

        case JLT:
          n = stack.back();
          stack.pop_back();

          y = stack.back();
          stack.pop_back();

          x = stack.back();
          stack.pop_back();

          if(x < y) {
            ip = n - 1;
          }

          break;

        case GET:
          n = stack.back();
          stack.pop_back();

          stack.push_back(stack[(stack.size() - 1) - n]);
          break;

        case SET:
          n = stack.back();
          stack.pop_back();

          x = stack.back();
          stack.pop_back();

          stack[(stack.size() - 1) - n] = x;
          break;

        case DUP:
          stack.push_back(stack.back());
          break;

        case POP:
          stack.pop_back();
          break;

        case PPOS:
          stack.push_back(ip);
          break;

        case SIZE:
          stack.push_back(stack.size());
          break;

        case SWAP:
          y = stack.back();
          stack.pop_back();

          x = stack.back();
          stack.pop_back();

          stack.push_back(y);
          stack.push_back(x);
          break;

        case DBG:
          printStack();
          break;

        case PRINT:
          std::cout << stack.back() << std::endl;
          stack.pop_back();
          break;

        case CPRINT:
          std::cout << (char) stack.back() << std::endl;
          stack.pop_back();
          break;

        case READ:
          std::cin >> x;
          stack.push_back(x);
          break;

        case CREAD:
          stack.push_back(std::cin.get());
          break;
      }
    }

    ip++;
  }
}


void Interpreter::printStack() {
  std::cout << '[';

  for(int i = 0; i < stack.size(); i++) {
    std::cout << stack[i];

    if(i != stack.size() - 1) {
      std::cout << ", ";
    }
  }

  std::cout << ']';
}

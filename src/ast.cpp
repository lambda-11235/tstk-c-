
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "ast.h"


std::vector<Instruction> removeLabels(std::vector<Token> toks,
                                      size_t startAddr) {
  std::map<std::string, size_t> lblAddrs = labelAddresses(toks, startAddr);
  std::vector<Instruction> insts;
  Instruction tmpInst;

  for(Token tok : toks) {
    switch(tok.type) {
      case LABEL: break;
      case REFER:
        tmpInst.pushInt = true;
        tmpInst.inst.n = lblAddrs[tok.name];
        insts.push_back(tmpInst);
        break;
      case INST:
        insts.push_back(tok.inst);
        break;
    }
  }

  return insts;
}


std::map<std::string, size_t> labelAddresses(std::vector<Token> toks,
                                               size_t startAddr) {
  // The instruction pointer keeps the address of the current instruction.
  size_t ip = startAddr;
  std::map<std::string, size_t> lblAddrs;

  for(Token tok : toks) {
    switch(tok.type) {
      case LABEL:
        // Labels aren't instructions, so we don't increment ip.
        lblAddrs[tok.name] = ip;
        break;
      default:
        ip++;
        break;
    }
  }

  return lblAddrs;
}


void printInstruction(Instruction inst) {
  if(inst.pushInt) {
    std::cout << inst.inst.n;
  } else {
    switch(inst.inst.com) {
      case ADD: std::cout << "add"; break;
      case SUB: std::cout << "sub"; break;
      case MUL: std::cout << "mul"; break;
      case DIV: std::cout << "div"; break;
      case JMP: std::cout << "jmp"; break;
      case JEQ: std::cout << "jeq"; break;
      case JNQ: std::cout << "jnq"; break;
      case JGT: std::cout << "jgt"; break;
      case JLT: std::cout << "jlt"; break;
      case GET: std::cout << "get"; break;
      case SET: std::cout << "set"; break;
      case DUP: std::cout << "dup"; break;
      case POP: std::cout << "pop"; break;
      case PPOS: std::cout << "ppos"; break;
      case SIZE: std::cout << "size"; break;
      case SWAP: std::cout << "swap"; break;
      case DBG: std::cout << "dbg"; break;
      case PRINT: std::cout << "print"; break;
      case CPRINT: std::cout << "cprint"; break;
      case READ: std::cout << "read"; break;
      case CREAD: std::cout << "cread"; break;
      default: std::cout << "???"; break;
    }
  }
}

void printInstructions(std::vector<Instruction> insts) {
  for(Instruction inst : insts) {
    printInstruction(inst);
    std::cout << ' ';
  }
}

void printTokens(std::vector<Token> toks) {
  for(Token tok : toks) {
    switch(tok.type) {
      case LABEL:
        std::cout << ':' << tok.name << ": ";
        break;
      case REFER:
        std::cout << '@' << tok.name << " ";
        break;
      case INST:
        printInstruction(tok.inst);
        std::cout << " ";
        break;
      default:
        std::cout << "??? ";
        break;
    }
  }
}

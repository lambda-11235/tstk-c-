
#include <map>
#include <string>
#include <vector>

#include <stdlib.h>

#ifndef AST_H
#define AST_H

enum COMMAND : char {
  ADD, SUB, MUL, DIV,
  JMP, JEQ, JNQ, JGT, JLT,
  GET, SET,
  DUP, POP, PPOS, SIZE, SWAP, DBG,
  PRINT, CPRINT, READ, CREAD
};

/**
 * Stores either the instruction to push an integer or a command.
 */
struct Instruction {
  bool pushInt;

  union {
    size_t n;
    COMMAND com;
  } inst;
};


enum TOKENTYPE : char {LABEL, REFER, INST};

/**
 * Stores either a label with name, a reference to a label with name, or an
 * instruction inst.
 */
struct Token {
  TOKENTYPE type;

  // Unfortunately a union cannot be used here since string has a non-trivial
  // constructor. So, just ignore the field that isn't used.
  std::string name;
  Instruction inst;
};


/**
 * Replaces all label references with their appropriate addresses, and removes
 * all labels.
 *
 * @param toks The tokens to replace the labels of.
 * @param startAddr The address of the first token in the list.
 *
 * @return The instruction set after all labels have been removed.
 */
std::vector<Instruction> removeLabels(std::vector<Token> toks,
                                      size_t startAddr);

/**
 * Gets a map of the names of labels to the addresses they point to.
 */
std::map<std::string, size_t> labelAddresses(std::vector<Token> toks,
                                               size_t startAddr);

void printInstruction(Instruction inst);
void printInstructions(std::vector<Instruction> insts);
void printTokens(std::vector<Token> toks);

#endif /* AST_H */

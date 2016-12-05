// Programmer: Taran Lynn
// Programmer's ID: 1513043

#include <ostream>
#include <vector>

#include "../ast.h"
#include "Compiler.h"

#ifndef COMPILER_C
#define COMPILER_C

namespace compiler {
  class C : public Compiler {
    std::vector<Instruction> instructions;

  public:
    C();

    /// Adds the instructions in a set of tokens to be compiled.
    void addTokens(std::vector<Token> toks);

    /// Compiles the given TSTK code and writes the output to writer.
    void compile(std::ostream& writer, int_type stackSize);

  private:
    /// Compiles the lookup table.
    void compileLookup(std::ostream& writer);

    /// Compiles an optimized jump, which consists of an integer push
    /// instruction followed by a jump instruction.
    void compileOptJump(std::ostream& writer, int_type curAddr, int_type
      targetAddr, COMMAND command);

    /// Compiles one of ADD, SUB, MUL, or DIV.
    void compileArith(std::ostream& writer, COMMAND command);

    /// Compiles a condition jumo. One of JEQ, JNQ, JLT, or JGT.
    void compileCondJump(std::ostream& writer, COMMAND command);
  };
} /* compiler */

#endif /* end of include guard: COMPILER_C */

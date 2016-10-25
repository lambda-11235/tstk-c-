
#include <ostream>
#include <vector>

#include "../ast.h"

#ifndef COMPILER
#define COMPILER

namespace compiler {
  class Compiler {
  public:
    /// Adds the instructions in a set of tokens to be compiled.
    virtual void addTokens(std::vector<Token> toks) = 0;

    /// Compiles the given TSTK code and writes the output to writer.
    virtual void compile(std::ostream& writer, int_type stackSize) = 0;
  };
} /* compiler */

#endif /* end of include guard: COMPILER */

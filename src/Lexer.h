
#include <istream>
#include <string>
#include <vector>

#include "ast.h"
#include "exception/Lexer.h"


#ifndef LEXER_H
#define LEXER_H


class Lexer {
  std::string file;
  std::istream* input;
  int line;
  int column;

public:
  /**
   * Construct a new lexer that read input from a stream.
   */
  Lexer(std::string inputFile, std::istream* instr);

  /**
   * Produce a series of tokens from the input stream.
   */
  std::vector<Token> tokenize();

  /**
   * Get the next token in the input stream.
   */
  Token getToken();

private:
  void skipWhitespace();

  int_type getInteger();
  std::string getWord();

  inline char peek();
  inline char get();
};

#endif /* LEXER_H */

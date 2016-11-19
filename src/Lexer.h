
#include <istream>
#include <vector>

#include "ast.h"


#ifndef LEXER_H
#define LEXER_H

/**
 * A possible lexer error, contaning information about the line, column, and
 * details of the error.
 */
class LexerException {
  int line;
  int column;
  std::string msg;

public:
  LexerException(int line, int column, std::string msg)
    : line(line), column(column), msg(msg) {
  }

  inline int getLine() { return line; }
  inline int getColumn() { return column; }
  inline std::string getMessage() { return msg; }
};


class Lexer {
  std::istream* input;
  int line;
  int column;

public:
  /**
   * Construct a new lexer that read input from a stream.
   */
  Lexer(std::istream* instr);

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

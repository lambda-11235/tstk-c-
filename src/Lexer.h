
#include <istream>
#include <vector>

#include "ast.h"


#ifndef LEXER_H
#define LEXER_H

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
  Lexer(std::istream* instr);

  std::vector<Token> tokenize();
  Token getToken();

private:
  void skipWhitespace();

  size_t getInteger();
  std::string getWord();

  inline char peek();
  inline char get();
};

#endif /* LEXER_H */

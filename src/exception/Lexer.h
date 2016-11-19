
#include <stdexcept>
#include <string>

#ifndef EXCEPTION_LEXER_H
#define EXCEPTION_LEXER_H


/**
 * A possible lexer error, contaning information about the line, column, and
 * details of the error.
 */
class LexerException : public std::runtime_error {
  int line;
  int column;
  std::string msg;

public:
  LexerException(int line, int column, const std::string& msg)
    : line(line), column(column), msg(msg), runtime_error(msg) { }

  inline int getLine() const { return line; }
  inline int getColumn() const { return column; }
  inline std::string getMessage() const { return msg; }
};


#endif /* end of include guard: EXCEPTION_LEXER_H */

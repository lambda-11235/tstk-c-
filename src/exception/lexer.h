
#include <stdexcept>
#include <string>

#ifndef EXCEPTION_LEXER_H
#define EXCEPTION_LEXER_H


/**
 * A possible lexer error, contaning information about the line, column, and
 * details of the error.
 */
class LexerError {
  std::string file;
  int line;
  int column;
  std::string msg;

public:
  LexerError(const std::string& file, int line, int column, const std::string& msg)
    : file(file), line(line), column(column), msg(msg) { }

  inline std::string getFile() const { return file; }
  inline int getLine() const { return line; }
  inline int getColumn() const { return column; }
  inline std::string getMessage() const { return msg; }
};


#endif /* end of include guard: EXCEPTION_LEXER_H */

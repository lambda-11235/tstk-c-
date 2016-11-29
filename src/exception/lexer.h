
#include <sstream>
#include <stdexcept>
#include <string>

#ifndef EXCEPTION_LEXER_H
#define EXCEPTION_LEXER_H


/**
 * A possible lexer error, contaning information about the line, column, and
 * details of the error.
 */
class LexerError : std::exception {
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

  const char* what() const throw() {
    std::stringstream strm;

    strm << "Lexical error in " << file << " at line " << line << ", column "
      << column << ": " << msg;

    return strm.str().c_str();
  }
};


#endif /* end of include guard: EXCEPTION_LEXER_H */

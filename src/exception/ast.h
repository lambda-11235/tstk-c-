
#include <stdexcept>
#include <string>

#ifndef EXCEPTION_AST_H
#define EXCEPTION_AST_H


/**
 *
 */
class ReferenceError : public std::runtime_error {
  int line;
  int column;
  std::string reference;

public:
  ReferenceError(int line, int column, const std::string& reference)
    : line(line), column(column), reference(reference),
      runtime_error("Reference to unknown label: @" + reference) { }

  inline int getLine() const { return line; }
  inline int getColumn() const { return column; }
  inline std::string getReference() const { return reference; }
};


#endif /* end of include guard: EXCEPTION_AST_H */

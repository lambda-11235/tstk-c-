
#include <stdexcept>
#include <string>

#ifndef EXCEPTION_AST_H
#define EXCEPTION_AST_H


/**
 *
 */
class ReferenceError : public std::runtime_error {
  std::string reference;

public:
  ReferenceError(const std::string& reference)
    : reference(reference), runtime_error("Reference to unknown label: @"
      + reference) { }

  inline std::string getReference() const { return reference; }
};


#endif /* end of include guard: EXCEPTION_AST_H */

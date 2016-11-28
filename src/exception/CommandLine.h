
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#ifndef EXCEPTION_COMMAND_LINE_H
#define EXCEPTION_COMMAND_LINE_H


/**
 * A command line error that occurs when the user gives an inexistent flag.
 */
class NoFlagError {
private:
  std::string flag;

public:
  NoFlagError(const std::string& flag) : flag(flag) {}

  inline std::string getFlag() const { return flag; }
};


#endif /* end of include guard: EXCEPTION_COMMAND_LINE_H */

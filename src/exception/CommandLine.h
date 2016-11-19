
#include <stdexcept>
#include <string>
#include <vector>

#ifndef EXCEPTION_COMMAND_LINE_H
#define EXCEPTION_COMMAND_LINE_H


/**
 * Any error in parsing the command line arguments.
 */
class CommandLineError : public std::runtime_error {
public:
  CommandLineError(const std::string& error) : runtime_error(error) {}
};


/**
 * A command line error that occurs when the user gives an inexistent flag.
 */
class NoFlagError : public CommandLineError {
private:
  std::string flag;

public:
  NoFlagError(const std::string& flag) : CommandLineError("Invalid command line flag "
    + flag), flag(flag) {}

  inline std::string getFlag() const { return flag; }
};


#endif /* end of include guard: EXCEPTION_COMMAND_LINE_H */

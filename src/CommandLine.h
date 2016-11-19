
#include <stdexcept>
#include <string>
#include <vector>

#include "ast.h"

#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

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
  NoFlagError(std::string flag) : CommandLineError("Invalid command line flag "
    + flag), flag(flag) {}

  inline std::string getFlag() const { return flag; }
};


/**
 * A class to parse command line arguments.
 */
class CommandLine {
private:
  bool interpret;
  bool help;
  bool compile;
  int stackSize;
  std::string outputFile;
  std::vector<std::string> inputFiles;

  bool expectingStackSize;
  bool expectingOutputFile;
  std::vector<std::string> args;

public:
  CommandLine(int argc, char* argv[]);

  inline bool shouldInterpret() { return interpret; }
  inline bool shouldPrintHelp() { return help; }
  inline bool shouldCompile() { return compile; }
  inline int_type getStackSize() { return stackSize; }

  inline bool hasOutputFile() { return outputFile != ""; }
  inline std::string getOutputFile() { return outputFile; }

  inline bool hasInputFiles() { return inputFiles.size() > 0; }
  inline std::vector<std::string> getInputFiles() { return inputFiles; }

  void parse();

private:
  void parseLongFlag(std::string flag);
  void parseShortFlags(std::string flags);
};

#endif // COMMAND_LINE_H

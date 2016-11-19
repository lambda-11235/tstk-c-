
#include <string>
#include <vector>

#include "ast.h"
#include "exception/CommandLine.h"

#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H


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

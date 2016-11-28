
#include <string>
#include <vector>

#include "ast.h"
#include "exception/commandLine.h"

#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H


/**
 * A class to parse command line arguments.
 */
class CommandLine {
private:
  bool debug;
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

  inline bool shouldDebug() const { return debug; }
  inline bool shouldInterpret() const { return interpret; }
  inline bool shouldPrintHelp() const { return help; }
  inline bool shouldCompile() const { return compile; }
  inline int_type getStackSize() const { return stackSize; }

  inline bool hasOutputFile() const { return outputFile != ""; }
  inline std::string getOutputFile() const { return outputFile; }

  inline bool hasInputFiles() const { return inputFiles.size() > 0; }
  inline std::vector<std::string> getInputFiles() const { return inputFiles; }

  void parse();

private:
  void parseLongFlag(std::string flag);
  void parseShortFlags(std::string flags);
};

#endif // COMMAND_LINE_H

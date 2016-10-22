
#include <stdexcept>
#include <string>
#include <vector>

#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

class CommandLineError : public std::runtime_error {
public:
  CommandLineError(const std::string& error) : runtime_error(error) {}
};

class NoFlagError : public CommandLineError {
private:
  std::string flag;

public:
  NoFlagError(std::string flag) : CommandLineError("Invalid command line flag "
    + flag), flag(flag) {}
};


class CommandLine {
private:
  bool interpret;
  bool help;
  std::vector<std::string> inputFiles;

  std::vector<std::string> args;

public:
  CommandLine(int argc, char* argv[]);

  inline bool shouldInterpret() { return interpret; }
  inline bool shouldPrintHelp() { return help; }
  inline bool hasInputFiles() { return inputFiles.size() > 0; }
  inline std::vector<std::string> getInputFiles() { return inputFiles; }

  void parse();

private:
  void parseLongFlag(std::string flag);
  void parseShortFlags(std::string flags);
};

#endif // COMMAND_LINE_H

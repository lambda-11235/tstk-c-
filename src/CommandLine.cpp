
#include "CommandLine.h"


CommandLine::CommandLine(int argc, char* argv[]) {
  // Defaults
  interpret = false;
  help = false;
  compile = false;
  stackSize = 30000;
  outputFile = "";

  for(int i = 1; i < argc; i++) {
    args.push_back(argv[i]);
  }
}


void CommandLine::parse() {
  expectingStackSize = false;

  bool onlyInputFiles = false;

  // If no arguments are passed, then run the interpreter.
  if(args.size() == 0) {
    interpret = true;
  }

  for(std::string arg : args) {
    if(arg.size() == 0) {
      continue;
    } else if(onlyInputFiles) {
      inputFiles.push_back(arg);
    } else if(arg[0] == '-') {
      if(arg.size() >= 2 && arg[1] == '-') {
        if(arg.size() == 2)
          onlyInputFiles = true;
        else
          parseLongFlag(arg.substr(2));
      } else {
        parseShortFlags(arg.substr(1));
      }
    } else if(expectingStackSize) {
      stackSize = atoi(arg.c_str());
      expectingStackSize = false;
    } else if(expectingOutputFile) {
      outputFile = arg;
      expectingOutputFile = false;
    } else {
      inputFiles.push_back(arg);
    }
  }
}


void CommandLine::parseLongFlag(std::string flag) {
  if(flag == "compile")
    compile = true;
  else if(flag == "interpret")
    interpret = true;
  else if(flag == "help")
    help = true;
  else if(flag == "output")
    expectingOutputFile = true;
  else if(flag == "stack-size")
    expectingStackSize = true;
  else
    throw NoFlagError(flag);
}


void CommandLine::parseShortFlags(std::string flags) {
  for(char flag : flags) {
    switch(flag) {
      case 'c':
        compile = true;
        break;

      case 'i':
        interpret = true;
        break;

      case 'h':
        help = true;
        break;

      case 'o':
        expectingOutputFile = true;
        break;

      case 's':
        expectingStackSize = true;
        break;

      default:
        std::string str;
        str += flag;
        throw NoFlagError(str);
    }
  }
}

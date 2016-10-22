
#include "CommandLine.h"


CommandLine::CommandLine(int argc, char* argv[]) {
  // Defaults
  interpret = false;
  help = false;

  for(int i = 1; i < argc; i++) {
    args.push_back(argv[i]);
  }
}


void CommandLine::parse() {
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
    } else {
      inputFiles.push_back(arg);
    }
  }
}


void CommandLine::parseLongFlag(std::string flag) {
  if(flag == "interpret")
    interpret = true;
  else if(flag == "help")
    help = true;
  else
    throw NoFlagError(flag);
}


void CommandLine::parseShortFlags(std::string flags) {
  for(char flag : flags) {
    switch(flag) {
      case 'i':
        interpret = true;
        break;
      case 'h':
        help = true;
        break;
      default:
        std::string str;
        str += flag;
        throw NoFlagError(str);
    }
  }
}

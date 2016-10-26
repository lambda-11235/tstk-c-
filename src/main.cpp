
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "ast.h"
#include "CommandLine.h"
#include "Lexer.h"
#include "Interpreter.h"
#include "compiler/C.h"


void printHelpMsg();


int main(int argc, char *argv[]) {
  Interpreter interpreter;
  vector<Token> toks;

  CommandLine commandLine(argc, argv);

  try {
    commandLine.parse();
  } catch(CommandLineError& err) {
    cout << "Error: " << err.what() << ".\n";
    return 1;
  }


  if(commandLine.shouldPrintHelp()) {
    printHelpMsg();
    return 0;
  }


  if(commandLine.hasInputFiles()) {
    for(string file : commandLine.getInputFiles()) {
      ifstream input(file.c_str());

      if(input) {
        Lexer lex(&input);

        try {
          for(Token tok : lex.tokenize())
          toks.push_back(tok);
        } catch(LexerException& lexExc) {
          cout << file << ' ' << lexExc.getLine() << ','
          << lexExc.getColumn() << ": " << lexExc.getMessage() << ".\n";

          return 1;
        }

        input.close();
      } else {
        cout << "Error: Could not read file " << file << ".\n";

        return 1;
      }
    }

    if(commandLine.shouldCompile()) {
      compiler::C compiler;
      compiler.addTokens(toks);

      if(commandLine.hasOutputFile()) {
        ofstream writer(commandLine.getOutputFile().c_str());
        compiler.compile(writer, commandLine.getStackSize());
      } else {
        compiler.compile(cout, commandLine.getStackSize());
      }
    } else {
      interpreter.addTokens(toks);
      interpreter.run();
    }
  }


  if(commandLine.shouldInterpret()) {
    string buf;
    stringstream strm;

    while(cin) {
      getline(cin, buf);
      strm.clear();
      strm.str(buf);

      Lexer lex(&strm);
      toks.clear();

      try {
        for(Token tok : lex.tokenize())
          toks.push_back(tok);
      } catch(LexerException& lexExc) {
        cout << lexExc.getColumn() << ": " << lexExc.getMessage() << endl;
      }

      interpreter.addTokens(toks);
      interpreter.run();

      interpreter.printStack();
      cout << endl;
    }
  }

  return 0;
}


void printHelpMsg() {
  cout << "Usage: tstk [files] [-o file] [-s stack_size]\n\n"
       << "Flags:\n"
       << "\t-c, --compile -- Compile listed files instead of interpreting them.\n"
       << "\t-i, --interpret -- Run the interpreter.\n"
       << "\t-h, --help -- Print this help message.\n"
       << "\t-o, --output -- Set the output file for compilation.\n"
       << "\t-s, --stack-size -- Set the stack size in compiled code.\n";
}

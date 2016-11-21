
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

      try{
        compiler.addTokens(toks);
      } catch(ReferenceError& err) {
        cerr << "Error: Reference to unknown label, @" << err.getReference() << endl;
        return 1;
      }

      if(commandLine.hasOutputFile()) {
        ofstream writer(commandLine.getOutputFile().c_str());
        compiler.compile(writer, commandLine.getStackSize());
      } else {
        compiler.compile(cout, commandLine.getStackSize());
      }
    }

    else if(commandLine.shouldDebug()) {
      Instruction inst;
      string buf;
      interpreter.addTokens(toks);

      cout << "Running script (press enter to execute instructions)";

      while(interpreter.stillRunning()) {
        inst = interpreter.currentInstruction();
        cout << "\n<Running> ";
        printInstruction(inst);

        getline(cin, buf);

        interpreter.runInstruction();

        if(!inst.pushInt && (inst.inst.com == READ || inst.inst.com == CREAD))
          cin.ignore();

        if(!inst.pushInt && inst.inst.com == CPRINT) cout << endl;
        interpreter.printStack();
      }
    }

    else {
      interpreter.addTokens(toks);
      interpreter.run();
    }
  }


  if(commandLine.shouldInterpret()) {
    string buf;
    stringstream strm;

    while(cin) {
      cout << "> ";
      getline(cin, buf);
      strm.clear();
      strm.str(buf);

      Lexer lex(&strm);
      toks.clear();

      try {
        for(Token tok : lex.tokenize())
          toks.push_back(tok);
      } catch(LexerException& lexExc) {
        // The line isn't printed since it was obviously the last one entered
        // into the REPL.
        cout << lexExc.getColumn() << ": " << lexExc.getMessage() << endl;
      }

      try{
        interpreter.addTokens(toks);
        interpreter.run();

        interpreter.printStack();
        cout << endl;
      } catch(ReferenceError& err) {
        cerr << "Error: Reference to unknown label, @" << err.getReference() << endl;
      }
    }
  }

  return 0;
}


void printHelpMsg() {
  cout << "Usage: tstk [flags] [files] [-o file] [-s stack_size]\n\n"
       << "Flags:\n"
       << "\t-c, --compile -- Compile listed files instead of interpreting them.\n"
       << "\t-d, --debug -- Run the debugger.\n"
       << "\t-i, --interpret -- Run the interpreter.\n"
       << "\t-h, --help -- Print this help message.\n"
       << "\t-o, --output -- Set the output file for compilation.\n"
       << "\t-s, --stack-size -- Set the stack size in compiled code.\n";
}

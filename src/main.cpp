
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
  } catch(NoFlagError& err) {
    cout << "Error: Unrecognized flag '" << err.getFlag() << "'.\n";
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
        Lexer lex(file, &input);

        try {
          for(Token tok : lex.tokenize())
          toks.push_back(tok);
        } catch(LexerError& err) {
          cout << err.getFile() << ' ' << err.getLine() << ','
            << err.getColumn() << ": " << err.getMessage() << ".\n";

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
        cerr << "Error: Reference to unknown label (" << err.getFile() << ' '
          << err.getLine() << ":" << err.getColumn() << "): @"
          << err.getReference() << endl;

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
      Token tok;
      string buf;

      try {
       interpreter.addTokens(toks);
      } catch(ReferenceError& err) {
        cerr << "Error: Reference to unknown label (" << err.getFile() << ' '
          << err.getLine() << ':' << err.getColumn() << "): @"
          << err.getReference() << endl;

        return 1;
      }

      cout << "Running script (press enter to execute instructions)\n";

      while(interpreter.stillRunning()) {
        inst = interpreter.currentInstruction();
        tok = interpreter.currentToken();

        cout << "<Running " << tok.file << " line " << tok.line << ", column "
          << tok.column << "> ";
        printToken(tok);

        getline(cin, buf);

        try {
          interpreter.runInstruction();
        } catch(StackUnderflow& err) {
          cerr << "Error: Stack underflow.\n";
          return 1;
        }

        if(!inst.pushInt && (inst.inst.com == READ || inst.inst.com == CREAD))
          cin.ignore();

        if(!inst.pushInt && inst.inst.com == CPRINT) cout << endl;
        interpreter.printStack();
      }
    }

    else {
      try{
        interpreter.addTokens(toks);

        interpreter.run();
      } catch(ReferenceError& err) {
        cerr << "Error: Reference to unknown label (" << err.getFile() << ' '
          << err.getLine() << ':' << err.getColumn() << "): @"
          << err.getReference() << endl;

        return 1;
      } catch(StackUnderflow& err) {
        Token tok = interpreter.currentToken();
        cerr << "Error: Stack underflow at " << tok.file << " (" << tok.line
          << ':' << tok.column << ") when executing '";

        printToken(tok);

        cout << "'.\n";

        return 1;
      }
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

      Lexer lex("CIN", &strm);
      toks.clear();

      try {
        for(Token tok : lex.tokenize())
          toks.push_back(tok);
      } catch(LexerError& err) {
        // The line isn't printed since it was obviously the last one entered
        // into the REPL.
        cout << err.getColumn() << ": " << err.getMessage() << endl;
      }

      try{
        interpreter.addTokens(toks);
        interpreter.run();

        interpreter.printStack();
      } catch(ReferenceError& err) {
        cerr << "Error: Reference to unknown label (" << err.getColumn()
          << "): @" << err.getReference() << endl;
      } catch(StackUnderflow& err) {
        cerr << "Error: Stack underflow.\n";
        return 1;
      }
    }
  }

  return 0;
}


void printHelpMsg() {
  cout << "Usage: tstk [flags] [files] [-o file] [-s stack_size]\n\n"
       << "Flags:\n"
       << "  -c, --compile     -- Compile listed files instead of interpreting them.\n"
       << "  -d, --debug       -- Run the debugger.\n"
       << "  -i, --interpret   -- Run the interpreter.\n"
       << "  -h, --help        -- Print this help message.\n"
       << "  -o, --output      -- Set the output file for compilation.\n"
       << "  -s, --stack-size  -- Set the stack size in compiled code.\n";
}

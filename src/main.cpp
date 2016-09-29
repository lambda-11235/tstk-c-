
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "ast.h"
#include "Lexer.h"
#include "Interpreter.h"


int main(int argc, char *argv[]) {
  Interpreter interpreter;
  vector<Token> toks;

  if(argc > 1) {
    for(int i = 1; i < argc; i++) {
      ifstream input(argv[i]);
      Lexer lex(&input);

      try {
        for(Token tok : lex.tokenize())
          toks.push_back(tok);
      } catch(LexerException& lexExc) {
        cout << argv[i] << ' ' << lexExc.getLine() << ','
             << lexExc.getColumn() << ": " << lexExc.getMessage() << endl;
      }

      input.close();
    }

    interpreter.addTokens(toks);
    interpreter.run();
  } else {
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

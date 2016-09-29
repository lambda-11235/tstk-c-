
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "ast.h"
#include "Lexer.h"
#include "Interpreter.h"


int main(int argc, char *argv[]) {
  Interpreter interpreter;
  vector<Token> toks;

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

  printTokens(toks);
  cout << endl;

  printInstructions(removeLabels(toks, 0));
  cout << endl;

  interpreter.addTokens(toks);
  interpreter.run();

  return 0;
}

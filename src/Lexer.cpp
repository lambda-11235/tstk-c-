
#include <istream>
#include <vector>

#include "ast.h"
#include "Lexer.h"


Lexer::Lexer(std::istream* instr) {
  input = instr;
  line = 1;
  column = 1;
}


std::vector<Token> Lexer::tokenize() {
  std::vector<Token> toks;

  while(peek() > 0) {
    toks.push_back(getToken());
  }

  return toks;
}


Token Lexer::getToken() {
  Token tok;
  char ch;

  skipWhitespace();

  ch = peek();

  if(ch == ':') {
    get();

    std::string name = getWord();

    if(peek() == ':') {
      get();

      tok.type = LABEL;
      tok.name = name;
    } else {
      throw LexerException(line, column, "Expected ':'");
    }
  }

  else if(ch == '@') {
    get();

    std::string name = getWord();

    tok.type = REFER;
    tok.name = name;
  }

  else if(ch == '-' || (ch >= '0' && ch <= '9')) {
    int_type n = getInteger();

    Instruction inst;
    inst.pushInt = true;
    inst.inst.n = n;

    tok.type = INST;
    tok.inst = inst;
  }

  else if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
    std::string com = getWord();

    Instruction inst;
    inst.pushInt = false;

    if(com == "add") { inst.inst.com = ADD; }
    else if(com == "sub") { inst.inst.com = SUB; }
    else if(com == "mul") { inst.inst.com = MUL; }
    else if(com == "div") { inst.inst.com = DIV; }
    else if(com == "jmp") { inst.inst.com = JMP; }
    else if(com == "jeq") { inst.inst.com = JEQ; }
    else if(com == "jnq") { inst.inst.com = JNQ; }
    else if(com == "jgt") { inst.inst.com = JGT; }
    else if(com == "jlt") { inst.inst.com = JLT; }
    else if(com == "get") { inst.inst.com = GET; }
    else if(com == "set") { inst.inst.com = SET; }
    else if(com == "dup") { inst.inst.com = DUP; }
    else if(com == "pop") { inst.inst.com = POP; }
    else if(com == "ppos") { inst.inst.com = PPOS; }
    else if(com == "size") { inst.inst.com = SIZE; }
    else if(com == "swap") { inst.inst.com = SWAP; }
    else if(com == "dbg") { inst.inst.com = DBG; }
    else if(com == "print") { inst.inst.com = PRINT; }
    else if(com == "cprint") { inst.inst.com = CPRINT; }
    else if(com == "read") { inst.inst.com = READ; }
    else if(com == "cread") { inst.inst.com = CREAD; }
    else {
      std::string msg = "Unexpected command ";
      msg += com;
      msg += " found";
      throw LexerException(line, column, msg);
    }

    tok.type = INST;
    tok.inst = inst;
  }

  else {
    std::string msg = "Unexpected character ";
    msg += ch;
    msg += " found";
    throw LexerException(line, column, msg);
  }

  return tok;
}


void Lexer::skipWhitespace() {
  while(peek() == ' ' || peek() == '\t' || peek() == '\r' || peek() == '\n'
        || peek() == '#') {
    // If we are on the start of a comment skip to the end of the line. For
    // all other whitespace we just skip a single character.
    if(peek() == '#')
      while(get() != '\n');
    else get();
  }
}


int_type Lexer::getInteger() {
  bool negative = false;
  int_type n = 0;
  int_type tmp;

  if(peek() == '-') {
    negative = true;
    get();
  }

  if(! (peek() >= '0' && peek() <= '9')) {
    throw LexerException(line, column, "Expecting a digit (0-9)");
  }

  while(peek() >= '0' && peek() <= '9') {
    tmp = get() - '0';
    n = n*10 + tmp;
  }

  return n;
}


std::string Lexer::getWord() {
  std::string word;

  while((peek() >= 'a' && peek() <= 'z') || (peek() >= 'A' && peek() <= 'Z')) {
    word += get();
  }

  return word;
}


char Lexer::peek() {
  return input->peek();
}


char Lexer::get() {
  if(peek() == '\n') {
    line++;
    column = 1;
  } else {
    column++;
  }

  return input->get();
}

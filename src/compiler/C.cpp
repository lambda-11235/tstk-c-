
#include <ostream>
#include <vector>

#include "../ast.h"
#include "Compiler.h"
#include "C.h"

namespace compiler {
  C::C() {}


  void C::addTokens(std::vector<Token> toks) {
    std::vector<Instruction> insts = removeLabels(toks, instructions.size());

    // Avoid extra copying of elements.
    instructions.reserve(instructions.size() + insts.size());

    for(Instruction inst : insts) {
      instructions.push_back(inst);
    }
  }


  void C::compile(std::ostream& writer, int_type stackSize) {
    Instruction curInst;
    Instruction nextInst;

    writer << "#include <runtime.h>\n\n"
           << "int main(int argc, char* argv[]) {\n"
           << "\tint_type size = 0;\n"
           << "\tint_type stack[" << stackSize << "];\n"
           << "\tint_type x;\n"
           << "\tint_type y;\n"
           << "\tint_type addr = 0;\n";

    compileLookup(writer);

    for(int i = 0; i < instructions.size(); i++) {
      writer << "L" << i << ":\n";

      curInst = instructions[i];

      if(curInst.pushInt) {
        // Compilation for an integer push checks whether the next instruction
        // is a jump, and is so does the jump optimization.
        if(i < instructions.size() - 1) {
          nextInst = instructions[i + 1];
        }

        if(i < instructions.size() - 1 && !nextInst.pushInt &&
            (nextInst.inst.com == JMP
              || nextInst.inst.com == JEQ || nextInst.inst.com == JNQ
              || nextInst.inst.com == JLT || nextInst.inst.com == JGT)) {
          compileOptJump(writer, i, curInst.inst.n, nextInst.inst.com);
        } else {
          writer << "\tstack[size] = " << curInst.inst.n << ";\n"
                 << "\tsize++;\n";
        }
      }

      else {
        switch(curInst.inst.com) {
          case ADD:
          case SUB:
          case MUL:
          case DIV:
            compileArith(writer, curInst.inst.com);
            break;

          case DUP:
            writer << "\tstack[size] = stack[size - 1];\n"
                   << "\tsize++;\n";
            break;

          case JMP:
            writer << "\tsize--;\n"
                   << "\taddr = stack[size];\n"
                   << "\tgoto lookup;\n";
            break;

          case JEQ:
          case JNQ:
          case JLT:
          case JGT:
            compileCondJump(writer, curInst.inst.com);
            break;

          case GET:
            writer << "\tsize--;\n"
                   << "\tx = stack[size];\n"
                   << "\tstack[size] = stack[(size - 1) - x];\n"
                   << "\tsize++;\n";
            break;

          case SET:
            writer << "\tsize--;\n"
                   << "\tx = stack[size];\n"
                   << "\tsize--;\n"
                   << "\ty = stack[size];\n"
                   << "\tstack[(size - 1) - y] = x;\n"
                   << "\tsize++;\n";
            break;

          case POP:
            writer << "\tsize--;\n";
            break;

          case PPOS:
            writer << "\tstack[size] = " << i << ";\n"
                   << "\tsize++;\n";
            break;

          case PRINT:
            writer << "\tsize--;\n"
                   << "\tx = stack[size];\n"
                   << "\tprint(x);\n";
            break;

          case CPRINT:
            writer << "\tsize--;\n"
                   << "\tx = stack[size];\n"
                   << "\tcprint(x);\n";
            break;

          case READ:
            writer << "\tstack[size] = read();\n"
                   << "\tsize++;\n";
            break;

          case CREAD:
            writer << "\tstack[size] = cread();\n"
                   << "\tsize++;\n";
            break;

          case SIZE:
            writer << "\tstack[size] = size;\n"
                   << "\tsize++;\n";
            break;

          case SWAP:
            writer << "\tx = stack[size - 2];\n"
                   << "\tstack[size - 2] = stack[size - 1];\n"
                   << "\tstack[size - 1] = x;\n";
            break;

          case DBG:
            writer << "\tdbg(stack, size);\n";
            break;
        }
      }
    }

    // In case there is a label at the end of the compiled TSTK code.
    writer << "L" << instructions.size() << ":\n"
           << "\treturn 0;\n"
           << "}";
  }


  void C::compileLookup(std::ostream& writer) {
    writer << "\tlookup:\n"
           << "\tswitch(addr) {\n";

    // Note: The <= is not a mistake. It's there to make sure there's a label at
    // the end of the code, as that is a valid place to jump to.
    for(int i = 0; i <= instructions.size(); i++) {
      writer << "\t\tcase " << i << ": goto L" << i << ";\n";
    }

    writer << "\t}\n";
  }


  void C::compileOptJump(std::ostream& writer, int_type curAddr,
    int_type targetAddr, COMMAND command) {
    if(command == JMP) {
      writer << "\tgoto L" << targetAddr << ";\n";
    } else {
      writer << "\tsize--;\n"
             << "\tx = stack[size];\n"
             << "\tsize--;\n"
             << "\ty = stack[size];\n"
             << "\tif(y ";

      switch(command) {
        case JEQ:
          writer << "==";
          break;

        case JNQ:
          writer << "!=";
          break;

        case JLT:
          writer << "<";
          break;

        case JGT:
          writer << ">";
          break;
      }

      writer << " x) {\n"
             << "\t\tgoto L" << targetAddr << ";\n"
             << "\t} else {\n"
             << "\t\tgoto L" << curAddr + 2 << ";\n"
             << "\t}\n";
    }
  }


  void C::compileArith(std::ostream& writer, COMMAND command) {
      writer << "\tsize--;\n"
             << "\tx = stack[size];\n"
             << "\tsize--;\n"
             << "\ty = stack[size];\n"
             << "\tstack[size] = y ";

    switch(command) {
      case ADD:
        writer << "+";
        break;

      case SUB:
        writer << "-";
        break;

      case MUL:
        writer << "*";
        break;

      case DIV:
        writer << "/";
        break;
    }

    writer << " x;\n"
           << "\tsize++;\n";
  }


  void C::compileCondJump(std::ostream& writer, COMMAND command) {
    writer << "\tsize--;\n"
           << "\tx = stack[size];\n"
           << "\tsize--;\n"
           << "\ty = stack[size];\n"
           << "\tsize--;\n"
           << "\tif(y ";

    switch(command) {
      case JEQ:
        writer << "==";
        break;

      case JNQ:
        writer << "!=";
        break;

      case JLT:
        writer << "<";
        break;

      case JGT:
        writer << ">";
        break;
    }

    writer << " x) {\n"
           << "\t\taddr = stack[size];\n"
           << "\t\tgoto lookup;\n"
           << "\t}\n";
  }
} /* compiler */

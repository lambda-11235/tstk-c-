add_executable(tstk
  main.cpp
  ast.h ast.cpp
  CommandLine.h CommandLine.cpp
  Lexer.h Lexer.cpp
  Interpreter.h Interpreter.cpp
  Stack.h

  compiler/Compiler.h
  compiler/C.h compiler/C.cpp

  exception/ast.h
  exception/commandLine.h
  exception/lexer.h
  exception/stack.h
  )

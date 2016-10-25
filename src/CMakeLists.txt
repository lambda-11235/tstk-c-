
add_executable(tstk
  main.cpp
  ast.h ast.cpp
  CommandLine.h CommandLine.cpp
  Lexer.h Lexer.cpp
  Interpreter.h Interpreter.cpp

  compiler/Compiler.h
  compiler/C.h compiler/C.cpp
  )

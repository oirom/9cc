#include "mcc.h"

int main(int argc, char **argv) {
  if (argc != 2)
    error("%s: invalid number of arguments", argv[0]);

  // Tokenize and parse.
  user_input = argv[1];
  token = tokenize();
  program();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // Traverse the AST to emit assembly.
  // ここでcode[i]に対して全部 codegen(code[i]);
  for(int i = 0; code[i]; ++i) {
    codegen(code[i]);
    printf("  pop rax\n");
  }

  // A result must be at the top of the stack, so pop it
  // to RAX to make it a program exit code.

  // printf("  pop rax\n");
  printf("  ret\n");

  // codegen(node);
  return 0;
}
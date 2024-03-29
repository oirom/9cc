/* Copyright 2021 oirom. All rights reserved. */
#include "./mcc.h"

int labelseq = 1;

void gen_lval(Node *node) {
  if (node->kind != ND_LVAR)
    error("lhs is not a variable");

  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}

void gen(Node *node) {
  switch (node->kind) {
  case ND_NUM:
    printf("  push %d\n", node->val);
    return;
  case ND_IF: {
    int seq = labelseq++;
    printf("# %s (%d)\n", __FILE__, __LINE__);
    gen(node->cond);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je  .Lelse%d\n", seq);
    gen(node->then);
    printf("  jmp .Lend%d\n", seq);
    printf(".Lelse%d:\n", seq);
    if (node->els) {
      gen(node->els);
    }
    printf(".Lend%d:\n", seq);
    return;
  }
  case ND_WHILE: {
    int seq_start = labelseq++;
    int seq_end = labelseq++;
    printf(".Lwhile%d:\n", seq_start);
    gen(node->cond);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je  .Lend%d\n", seq_end);
    gen(node->then);
    printf("  jmp .Lwhile%d\n", seq_start);
    printf(".Lend%d:\n", seq_end);
    return;
  }
  case ND_BLOCK:
    for (Node *n = node->body; n; n = n->next)
      gen(n);
    return;
  case ND_RETURN:
    printf("# %s (%d)\n", __FILE__, __LINE__);
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;
  case ND_LVAR:
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  case ND_ASSIGN:
    gen_lval(node->lhs);
    gen(node->rhs);

    // address of node->lhs
    // value of node->rhs

    printf("  pop rdi\n");  // value of node->rhs
    printf("  pop rax\n");  // address of node->lhs
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    // a = 10
    // return b = a + 9;

    // a = 10
    // b = c = a
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
  case ND_ADD:
    printf("  add rax, rdi\n");
    break;
  case ND_SUB:
    printf("  sub rax, rdi\n");
    break;
  case ND_MUL:
    printf("  imul rax, rdi\n");
    break;
  case ND_DIV:
    printf("  cqo\n");
    printf("  idiv  rdi\n");
    break;
  case ND_EQ:
    printf("  cmp rax, rdi\n");
    printf("  sete al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_NE:
    printf("  cmp rax, rdi\n");
    printf("  setne al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LT:
    printf("  cmp rax, rdi\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LE:
    printf("  cmp rax, rdi\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    // oirom0528 is awesome!
    break;
  }

  printf("  push rax\n");
}

void codegen(Node *node) {
  gen(node);
}

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// tokenize.c
//

typedef enum {
  TK_RESERVED,  // Keywords or punctuators
  TK_IDENT,     // Identifier
  TK_NUM,       // Integer literals
  TK_EOF,       // End-of-file markers
} TokenKind;

// Token Type
typedef struct Token Token;
struct Token {
  TokenKind kind; // Token kind
  Token *next;    // Next token
  int val;        // If kind is TK_NUM, its value
  char *str;      // Token string
  int len;        // Token length
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize();

extern char *user_input;
extern Token *token;

//
// parse.c
//

typedef struct LVar LVar;

struct LVar {
  LVar *next;
  char *name; // name
  int len;    // length of name
  int offset; // offset from RBP
};

typedef enum {
  ND_ADD,     // +
  ND_SUB,     // -
  ND_MUL,     // *
  ND_DIV,     // /
  ND_ASSIGN,  // =
  ND_LVAR,    // Local variable
  ND_EQ,      // ==
  ND_NE,      // !=
  ND_LT,      // <
  ND_LE,      // <=
  ND_IF,      // "if"
  ND_RETURN,  // "return"
  ND_NUM,     // Integer
} NodeKind;

// AST node type
typedef struct Node Node;

struct Node {
  NodeKind kind; // Node kind
  Node *lhs;     // Left-hand side
  Node *rhs;     // Right-hand side
  Node *cond;
  Node *then;
  Node *els;
  int val;       // Used if kind == ND_NUM
  int offset;    // kind
};

Node *expr();

extern Node *code[100];

//
// codegen.c
//

void codegen(Node *node);
void program(void);
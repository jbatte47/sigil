#include "ast.h"
#include "sigil.tab.h"
#include <stdio.h>
#include <stdlib.h>

extern ASTNode *root;
extern int yyparse();
extern FILE *yyin;

int main(int argc, char **argv) {
  if (argc > 1) {
    FILE *file = fopen(argv[1], "r");
    if (!file) {
      perror("fopen");
      return 1;
    }
    yyin = file;
  }

  printf("[SIGIL COMPILER] v2.0-alpha Parsing...\n");

  if (yyparse() == 0) {
    printf("[SIGIL COMPILER] Parse successful. AST Output:\n");
    print_ast(root, 0);
  } else {
    printf(
        "\n[SIGIL COMPILER] Ritual failed due to grammatical instability.\n");
    return 1;
  }

  return 0;
}

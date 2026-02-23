#include "ast.h"
#include "runtime.h"
#include "semantic.h"
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

  if (yyparse() == 0) {
    if (validate_ritual(root)) {
      execute_ritual(root);
    } else {
      printf("\n[SIGIL COMPILER] Ritual failed semantic validation. Reality "
             "instability detected.\n");
      return 1;
    }
  } else {
    printf(
        "\n[SIGIL COMPILER] Ritual failed due to grammatical instability.\n");
    return 1;
  }

  return 0;
}

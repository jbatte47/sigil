#include "compiler_interface.h"
#include "sigil.tab.h"
#include <stdio.h>
#include <stdlib.h>

extern int yyparse();
extern FILE *yyin;
extern void *yy_scan_string(const char *);
extern void yy_delete_buffer(void *);
extern ASTNode *root;

ASTNode *parse_sigil_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file)
    return NULL;

  yyin = file;
  root = NULL;
  if (yyparse() == 0) {
    fclose(file);
    return root;
  }
  fclose(file);
  return NULL;
}

ASTNode *parse_sigil_string(const char *input) {
  void *buffer = yy_scan_string(input);
  root = NULL;
  if (yyparse() == 0) {
    yy_delete_buffer(buffer);
    return root;
  }
  yy_delete_buffer(buffer);
  return NULL;
}

void free_ast(ASTNode *node) {
  if (!node)
    return;
  free_ast(node->left);
  free_ast(node->right);
  free_ast(node->next);
  if (node->value)
    free(node->value);
  free(node);
}

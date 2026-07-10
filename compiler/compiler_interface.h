#ifndef COMPILER_INTERFACE_H
#define COMPILER_INTERFACE_H

#include "ast.h"

// Parse a Sigil file and return the AST root
ASTNode *parse_sigil_file(const char *filename);

// Parse a Sigil string and return the AST root
ASTNode *parse_sigil_string(const char *input);

// Free the AST
void free_ast(ASTNode *node);

#endif // COMPILER_INTERFACE_H

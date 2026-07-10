#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode *create_node(NodeType type, char *value, ASTNode *left,
                     ASTNode *right) {
  return create_node_loc(type, value, left, right, 0, 0);
}

ASTNode *create_node_loc(NodeType type, char *value, ASTNode *left,
                         ASTNode *right, int line, int column) {
  ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
  node->type = type;
  node->value = value ? strdup(value) : NULL;
  node->left = left;
  node->right = right;
  node->next = NULL;
  node->line = line;
  node->column = column;
  return node;
}

void add_next(ASTNode *list, ASTNode *node) {
  if (!list || !node)
    return;
  ASTNode *curr = list;
  while (curr->next) {
    curr = curr->next;
  }
  curr->next = node;
}

const char *type_to_string(NodeType type) {
  switch (type) {
  case NODE_PROGRAM:
    return "PROGRAM";
  case NODE_MANIFEST_DECL:
    return "MANIFEST_DECL";
  case NODE_INCANT_DECL:
    return "INCANT_DECL";
  case NODE_TYPE_DECL:
    return "TYPE_DECL";
  case NODE_INSTANCE_DECL:
    return "INSTANCE_DECL";
  case NODE_ATTRIBUTE_APPLY:
    return "ATTRIBUTE_APPLY";
  case NODE_MODULE_REF:
    return "MODULE_REF";
  case NODE_BLOCK:
    return "BLOCK";
  case NODE_PIPELINE:
    return "PIPELINE";
  case NODE_TERNARY:
    return "TERNARY";
  case NODE_BINOP:
    return "BINOP";
  case NODE_MEMBER_ACCESS:
    return "MEMBER_ACCESS";
  case NODE_RECORD_CALL:
    return "RECORD_CALL";
  case NODE_PREFIX_CALL:
    return "PREFIX_CALL";
  case NODE_LAMBDA:
    return "LAMBDA";
  case NODE_LIST_LITERAL:
    return "LIST_LITERAL";
  case NODE_LIST_ITEM_ALIAS:
    return "LIST_ITEM_ALIAS";
  case NODE_OBJECT_LITERAL:
    return "OBJECT_LITERAL";
  case NODE_FIELD:
    return "FIELD";
  case NODE_IDENTIFIER:
    return "IDENTIFIER";
  case NODE_LITERAL:
    return "LITERAL";
  case NODE_PARAM:
    return "PARAM";
  case NODE_EXPR_LIST:
    return "EXPR_LIST";
  default:
    return "UNKNOWN";
  }
}

void print_ast(ASTNode *node, int indent) {
  if (!node)
    return;
  for (int i = 0; i < indent; i++)
    printf("  ");
  printf("[%s] %s (L:%d, C:%d)\n", type_to_string(node->type),
         node->value ? node->value : "", node->line, node->column);
  if (node->left) {
    print_ast(node->left, indent + 1);
  }
  if (node->right) {
    print_ast(node->right, indent + 1);
  }
  if (node->next) {
    print_ast(node->next, indent);
  }
}

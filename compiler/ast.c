#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode *create_node(NodeType type, char *value, ASTNode *left,
                     ASTNode *right) {
  ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
  node->type = type;
  node->value = value ? strdup(value) : NULL;
  node->left = left;
  node->right = right;
  node->next = NULL;
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
  case NODE_INCANTATION:
    return "INCANTATION";
  case NODE_FLOW:
    return "FLOW";
  case NODE_ESSENCE_INVOCATION:
    return "ESSENCE_INVOCATION";
  case NODE_MEMBER_ACCESS:
    return "MEMBER_ACCESS";
  case NODE_IDENTIFIER:
    return "IDENTIFIER";
  case NODE_YIELD:
    return "YIELD";
  case NODE_PARAM:
    return "PARAM";
  case NODE_MANIFEST:
    return "MANIFEST";
  case NODE_IMBUE:
    return "IMBUE";
  case NODE_TRIGGER:
    return "TRIGGER";
  case NODE_INVOKE:
    return "INVOKE";
  case NODE_IMPORT:
    return "IMPORT";
  case NODE_EXPORT:
    return "EXPORT";
  case NODE_INSTANCE:
    return "INSTANCE";
  case NODE_CALL:
    return "CALL";
  case NODE_ARG:
    return "ARG";
  case NODE_LET:
    return "LET";
  case NODE_BINOP:
    return "BINOP";
  case NODE_LITERAL:
    return "LITERAL";
  case NODE_NAMED_ARG:
    return "NAMED_ARG";
  case NODE_GIVEN_BLOCK:
    return "GIVEN_BLOCK";
  case NODE_GESTURE_STMT:
    return "GESTURE_STMT";
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
  printf("[%s] %s\n", type_to_string(node->type),
         node->value ? node->value : "");
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

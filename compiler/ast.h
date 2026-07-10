#ifndef SIGIL_AST_H
#define SIGIL_AST_H

typedef enum {
  NODE_PROGRAM,
  NODE_MANIFEST_DECL,
  NODE_INCANT_DECL,
  NODE_ATTRIBUTE_APPLY,
  NODE_MODULE_REF,
  NODE_BLOCK,
  NODE_PIPELINE,
  NODE_TERNARY,
  NODE_BINOP,
  NODE_MEMBER_ACCESS,
  NODE_RECORD_CALL,
  NODE_PREFIX_CALL,
  NODE_LAMBDA,
  NODE_LIST_LITERAL,
  NODE_LIST_ITEM_ALIAS,
  NODE_OBJECT_LITERAL,
  NODE_FIELD,
  NODE_IDENTIFIER,
  NODE_LITERAL, // STRING, NUMBER, etc.
  NODE_PARAM,
  NODE_EXPR_LIST // generic list of expressions
} NodeType;

typedef struct ASTNode {
  NodeType type;
  char *value;
  struct ASTNode *left;
  struct ASTNode *right;
  struct ASTNode *next; // For lists
  int line;
  int column;
} ASTNode;

ASTNode *create_node(NodeType type, char *value, ASTNode *left, ASTNode *right);
ASTNode *create_node_loc(NodeType type, char *value, ASTNode *left, ASTNode *right, int line, int column);
void add_next(ASTNode *list, ASTNode *node);
void print_ast(ASTNode *node, int indent);
const char *type_to_string(NodeType type);

#endif

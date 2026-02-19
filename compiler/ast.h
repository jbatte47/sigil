#ifndef SIGIL_AST_H
#define SIGIL_AST_H

typedef enum {
  NODE_INCANTATION,
  NODE_FLOW,
  NODE_ESSENCE_INVOCATION,
  NODE_MEMBER_ACCESS,
  NODE_IDENTIFIER,
  NODE_YIELD,
  NODE_PARAM,
  NODE_MANIFEST,
  NODE_IMBUE,
  NODE_TRIGGER,
  NODE_INVOKE,
  NODE_IMPORT,
  NODE_EXPORT,
  NODE_INSTANCE,
  NODE_CALL,
  NODE_ARG,
  NODE_LET,
  NODE_BINOP,
  NODE_LITERAL,
  NODE_NAMED_ARG,
  NODE_GIVEN_BLOCK,
  NODE_GESTURE_STMT,
  NODE_EXPR_LIST
} NodeType;

typedef struct ASTNode {
  NodeType type;
  char *value;
  struct ASTNode *left;
  struct ASTNode *right;
  struct ASTNode *next; // For lists
} ASTNode;

ASTNode *create_node(NodeType type, char *value, ASTNode *left, ASTNode *right);
void add_next(ASTNode *list, ASTNode *node);
void print_ast(ASTNode *node, int indent);
const char *type_to_string(NodeType type);

#endif

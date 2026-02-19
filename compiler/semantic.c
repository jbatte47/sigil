#include "semantic.h"
#include "nodes.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Symbol {
  char *name;
  char *interface;
  struct Symbol *next;
} Symbol;

typedef struct Pattern {
  char *name;
  char *incant_name;
  struct Pattern *next;
} Pattern;

static Symbol *symbols = NULL;
static Pattern *patterns = NULL;

void add_symbol(char *name, char *interface) {
  Symbol *s = (Symbol *)malloc(sizeof(Symbol));
  s->name = strdup(name);
  s->interface = strdup(interface);
  s->next = symbols;
  symbols = s;
}

char *get_symbol_interface(char *name) {
  // Implicit built-ins
  if (strcmp(name, "World") == 0)
    return "World";
  if (strcmp(name, "Func") == 0)
    return "Func";
  if (strcmp(name, "User") == 0)
    return "User";
  if (strcmp(name, "Vector") == 0)
    return "Vector";
  if (strcmp(name, "Essence") == 0)
    return "Essence";
  if (strcmp(name, "Trigger") == 0)
    return "Trigger";
  if (strcmp(name, "Action") == 0)
    return "Action";

  // Constants
  if (strcmp(name, "MAX_FACTOR") == 0)
    return "Factor";
  if (strcmp(name, "MAX_DISTANCE") == 0)
    return "Distance";
  if (strcmp(name, "MIN_FACTOR") == 0)
    return "Factor";

  // Enums
  if (strcmp(name, "Left") == 0 || strcmp(name, "Right") == 0)
    return "Hand";
  if (strcmp(name, "Thumb") == 0 || strcmp(name, "Second") == 0 ||
      strcmp(name, "Third") == 0)
    return "Finger";

  Symbol *curr = symbols;
  while (curr) {
    if (strcmp(curr->name, name) == 0)
      return curr->interface;
    curr = curr->next;
  }
  return NULL;
}

void add_pattern(char *name, char *incant_name) {
  Pattern *p = (Pattern *)malloc(sizeof(Pattern));
  p->name = strdup(name);
  p->incant_name = strdup(incant_name);
  p->next = patterns;
  patterns = p;
}

bool validate_node(ASTNode *node) {
  if (!node)
    return true;

  if (node->type == NODE_INSTANCE) {
    add_symbol(node->value, node->left->value);
    // Handle decorators
    ASTNode *d = node->left->next;
    while (d) {
      if (d->type == NODE_CALL && strcmp(d->value, "with_clothing") == 0) {
        // If wearing rags, might become Combustible
        if (d->left && d->left->type == NODE_LITERAL &&
            strcmp(d->left->value, "rags") == 0) {
          add_symbol(node->value, "Combustible");
        }
      }
      d = d->next;
    }
  }

  if (node->type == NODE_LET) {
    add_symbol(node->value, "Expression");
  }

  if (node->type == NODE_PARAM) {
    add_symbol(node->value, node->left->value);
  }

  if (node->type == NODE_MANIFEST) {
    add_pattern(node->left->value, node->value);
  }

  if (node->type == NODE_IMBUE) {
    if (!get_symbol_interface(node->value)) {
      printf("[SEMANTIC ERROR] Undefined instance '%s' used in imbue.\n",
             node->value);
      return false;
    }
  }

  if (node->type == NODE_CALL || node->type == NODE_MEMBER_ACCESS) {
    // Enforce unnamed argument rule: only if 1 arg total or curried to 1
    // (Simplified for this DSL phase)
  }

  if (!validate_node(node->left))
    return false;
  if (!validate_node(node->right))
    return false;
  if (!validate_node(node->next))
    return false;

  return true;
}

bool validate_ritual(ASTNode *root) {
  symbols = NULL;
  patterns = NULL;
  init_reality_registry();
  return validate_node(root);
}

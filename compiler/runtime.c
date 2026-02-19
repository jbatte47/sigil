#include "runtime.h"
#include "lexicon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Instance {
  char *name;
  char *interface;
  char *decoration;
  struct Instance *next;
} Instance;

static Instance *world_instances = NULL;

void add_instance(char *name, char *interface) {
  Instance *inst = (Instance *)malloc(sizeof(Instance));
  inst->name = strdup(name);
  inst->interface = strdup(interface);
  inst->decoration = NULL;
  inst->next = world_instances;
  world_instances = inst;
}

void decorate_instance(char *name, char *decoration) {
  Instance *curr = world_instances;
  while (curr) {
    if (strcmp(curr->name, name) == 0) {
      curr->decoration = strdup(decoration);
      return;
    }
    curr = curr->next;
  }
}

char *manifest_expr(ASTNode *node) {
  if (!node)
    return "";
  char *buf = (char *)malloc(2048);
  buf[0] = '\0';

  if (node->type == NODE_IDENTIFIER) {
    return strdup(node->value);
  }
  if (node->type == NODE_LITERAL) {
    return strdup(node->value);
  }
  if (node->type == NODE_BINOP) {
    if (strcmp(node->value, "+") == 0) {
      sprintf(buf, "%s and %s", manifest_expr(node->left),
              manifest_expr(node->right));
    } else {
      sprintf(buf, "%s %s %s", manifest_expr(node->left), node->value,
              manifest_expr(node->right));
    }
    return buf;
  }
  if (node->type == NODE_CALL) {
    char *name = manifest_expr(node->left);
    if (node->right) {
      sprintf(buf, "%s while %s", name, manifest_expr(node->right));
    } else {
      sprintf(buf, "%s", name);
    }
    return buf;
  }
  if (node->type == NODE_NAMED_ARG) {
    char *val = manifest_expr(node->left);
    if (strcmp(node->value, "radius") == 0) {
      sprintf(buf, "extending to a radius of %s", val);
    } else if (strcmp(node->value, "hand") == 0 ||
               strcmp(node->value, "finger") == 0) {
      sprintf(buf, "with %s", lookup_lexicon(gesture_lexicon, val));
    } else {
      sprintf(buf, "%s set to %s", node->value, val);
    }
    return buf;
  }
  if (node->type == NODE_MEMBER_ACCESS) {
    // Handle Action.sweep, Trigger.gesture, etc.
    char *obj = manifest_expr(node->left);
    if (strcmp(obj, "Action") == 0 || strcmp(obj, "Trigger") == 0 ||
        strcmp(obj, "Essence") == 0) {
      return strdup(lookup_lexicon(gesture_lexicon, node->value));
    }
    sprintf(buf, "%s's %s", obj, node->value);
    return buf;
  }
  if (node->next) {
    sprintf(buf, "%s, %s", manifest_expr(node), manifest_expr(node->next));
    return buf;
  }
  return "something";
}

void manifest_node(ASTNode *node) {
  if (!node)
    return;

  if (node->type == NODE_INSTANCE) {
    add_instance(node->value, node->left->value);
    // Check for decorators in AST
    ASTNode *d = node->left->next;
    while (d) {
      if (d->type == NODE_CALL && strcmp(d->value, "with_clothing") == 0 &&
          d->left) {
        decorate_instance(node->value, d->left->value);
      }
      d = d->next;
    }

    char *desc = lookup_lexicon(interface_lexicon, node->left->value);
    Instance *inst = world_instances;
    if (inst && inst->decoration) {
      printf("Resting here is %s, %s, clad in %s.\n", node->value, desc,
             inst->decoration);
    } else {
      printf("Resting here is %s, %s.\n", node->value, desc);
    }
  }

  if (node->type == NODE_IMBUE) {
    printf("\nYou begin to weave patterns into the %s, binding its form to "
           "your will.\n",
           node->value);
  }

  if (node->type == NODE_FLOW) {
    // Improved diegetic flow
    if (node->left->type == NODE_MEMBER_ACCESS &&
        strcmp(node->left->left->value, "Essence") == 0) {
      char *essence = lookup_lexicon(essence_lexicon, node->left->value);
      printf("A faint glow emerges as %s begins to flow toward its target.\n",
             essence);
    }
  }

  if (node->type == NODE_GIVEN_BLOCK) {
    printf(
        "\nThe air thickens as you prepare to simulate the arcane flow...\n");
  }

  if (node->type == NODE_GESTURE_STMT) {
    printf("\nYou perform a focused gesture with the %s.\n", node->value);
    printf("A searing bolt of arcane energy erupts from the staff's tip!\n");
    printf("The goblins are caught in the shimmering heat and sent tumbling "
           "backward by the crushing wall of kinetic pressure.\n");
  }

  manifest_node(node->left);
  manifest_node(node->right);
  manifest_node(node->next);
}

void execute_ritual(ASTNode *root) {
  printf("\n--- AREA DESCRIPTION ---\n");
  printf("The chamber is silent, smelled of ozone and ancient wood.\n");
  manifest_node(root);
  printf("\n--- END OF LOG ---\n");
}

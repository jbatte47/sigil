#include "runtime.h"
#include "lexicon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Instance {
  char *name;
  char *interface;
  char *decoration;
  char *distance;
  int hp;
  struct Instance *next;
} Instance;

typedef struct Ritual {
  char *name;
  struct Ritual *next;
} Ritual;

typedef struct Enchantment {
  char *vessel;
  char *synergy;
  char *essence_type;
  int intensity;
  char *required_gesture;
  double targeting_radius;
  double sphere_radius;
  struct Enchantment *next;
} Enchantment;

typedef struct ActiveEssence {
  char *name;
  int intensity;
  struct ActiveEssence *next;
} ActiveEssence;

static Instance *world_instances = NULL;
static Ritual *rituals = NULL;
static Enchantment *enchantments = NULL;
static ActiveEssence *active_essences = NULL;
static int current_skill_level = 0;
static int current_intensity = 0;
static double current_sphere_radius = 0;
static double current_targeting_radius = 0;
static char *last_gesture_prose = NULL;

void add_active_essence(char *name, int intensity) {
  // Prevent duplicates
  ActiveEssence *curr = active_essences;
  while (curr) {
    if (strcmp(curr->name, name) == 0) {
      if (intensity > curr->intensity)
        curr->intensity = intensity;
      return;
    }
    curr = curr->next;
  }
  ActiveEssence *ae = (ActiveEssence *)malloc(sizeof(ActiveEssence));
  ae->name = strdup(name);
  ae->intensity = intensity;
  ae->next = active_essences;
  active_essences = ae;
}

int is_combustible(Instance *inst) {
  if (strcmp(inst->interface, "Combustible") == 0)
    return 1;
  if (inst->decoration && strstr(inst->decoration, "rags"))
    return 1;
  return 0;
}

void find_initial_intensity(ASTNode *node) {
  if (!node)
    return;
  if (node->type == NODE_NAMED_ARG && strcmp(node->value, "intensity") == 0) {
    if (strcmp(node->left->value, "MAX_FACTOR") == 0)
      current_intensity = 100;
    else if (node->left->type == NODE_LITERAL)
      current_intensity = atoi(node->left->value);
  }
  find_initial_intensity(node->left);
  find_initial_intensity(node->right);
  find_initial_intensity(node->next);
}

void find_initial_skill_level(ASTNode *node) {
  if (!node)
    return;
  if (node->type == NODE_GIVEN_BLOCK) {
    ASTNode *call = node->left;
    if (call && call->type == NODE_CALL) {
      ASTNode *arg = call->right;
      while (arg) {
        if (arg->type == NODE_NAMED_ARG &&
            strcmp(arg->value, "skill_level") == 0) {
          if (strcmp(arg->left->value, "MAX_FACTOR") == 0)
            current_skill_level = 100;
          else if (arg->left && arg->left->type == NODE_LITERAL)
            current_skill_level = atoi(arg->left->value);
        }
        arg = arg->next;
      }
    }
  }
  find_initial_skill_level(node->left);
  find_initial_skill_level(node->right);
  find_initial_skill_level(node->next);
}

void add_instance(char *name, char *interface, char *distance) {
  Instance *inst = (Instance *)malloc(sizeof(Instance));
  inst->name = strdup(name);
  inst->interface = strdup(interface);
  inst->decoration = NULL;
  inst->distance = distance ? strdup(distance) : NULL;
  inst->hp = (strcmp(interface, "Enemy") == 0) ? 100 : 0;
  inst->next = NULL;
  if (!world_instances)
    world_instances = inst;
  else {
    Instance *curr = world_instances;
    while (curr->next)
      curr = curr->next;
    curr->next = inst;
  }
}

void add_ritual(char *name) {
  Ritual *r = (Ritual *)malloc(sizeof(Ritual));
  r->name = strdup(name);
  r->next = NULL;
  if (!rituals)
    rituals = r;
  else {
    Ritual *curr = rituals;
    while (curr->next)
      curr = curr->next;
    curr->next = r;
  }
}

void add_enchantment(char *vessel, char *synergy, char *essence, int intensity,
                     char *required_gesture, double targeting_radius,
                     double sphere_radius) {
  Enchantment *e = (Enchantment *)malloc(sizeof(Enchantment));
  e->vessel = strdup(vessel);
  e->synergy = strdup(synergy);
  e->essence_type = essence ? strdup(essence) : strdup("arcane");
  e->intensity = intensity;
  e->required_gesture = required_gesture ? strdup(required_gesture) : NULL;
  e->targeting_radius = targeting_radius;
  e->sphere_radius = sphere_radius;
  e->next = enchantments;
  enchantments = e;
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
    if (strcmp(node->value, "find_target") == 0)
      return "target_enemy";
    if (strcmp(node->value, "cast_fireball") == 0)
      return "fireball";
    return strdup(node->value);
  }
  if (node->type == NODE_LITERAL) {
    if (strcmp(node->value, "MAX_FACTOR") == 0)
      return "100";
    return strdup(node->value);
  }

  if (node->type == NODE_CALL) {
    char *func = manifest_expr(node->left);
    ASTNode *arg = node->right;

    // Check if it's a "with object" call like ash_staff(...)
    if (arg && !arg->next && arg->type != NODE_NAMED_ARG) {
      char *inner = manifest_expr(arg);
      // Try matching "with function" or just the function
      char combined[2048];
      sprintf(combined, "%s(%s)", func, inner);
      const char *narrative = lookup_lexicon(gesture_lexicon, combined);
      if (strcmp(narrative, combined) != 0)
        return strdup(narrative);

      // Fallback: search for "against" in the inner and join
      if (strstr(inner, "against")) {
        char *res = (char *)malloc(2048);
        sprintf(res, "%s %s", inner, func);
        return res;
      }
    }

    sprintf(buf, "%s(", func);
    while (arg) {
      if (arg->type == NODE_NAMED_ARG) {
        strcat(buf, arg->value);
        strcat(buf, "=");
        strcat(buf, manifest_expr(arg->left));
      } else {
        strcat(buf, manifest_expr(arg));
      }
      arg = arg->next;
      if (arg)
        strcat(buf, ", ");
    }
    strcat(buf, ")");

    if (strstr(func, "trigger_intensity")) {
      double skill_factor =
          (double)current_skill_level * (double)current_skill_level / 10000.0;
      current_intensity = (int)((double)current_intensity * skill_factor);
    }

    const char *narrative = lookup_lexicon(gesture_lexicon, buf);
    if (strcmp(narrative, buf) != 0)
      return strdup(narrative);

    return buf;
  }

  if (node->type == NODE_BINOP) {
    if (strcmp(node->value, "+") == 0) {
      sprintf(buf, "%s, while %s", manifest_expr(node->left),
              manifest_expr(node->right));
    } else {
      sprintf(buf, "%s %s %s", manifest_expr(node->left), node->value,
              manifest_expr(node->right));
    }
    return buf;
  }

  if (node->type == NODE_MEMBER_ACCESS) {
    char *obj = manifest_expr(node->left);
    char *tech = (char *)malloc(128);
    sprintf(tech, "%s.%s", obj, node->value);
    const char *narrative = lookup_lexicon(gesture_lexicon, tech);
    if (strcmp(narrative, tech) != 0)
      return strdup(narrative);
    return tech;
  }

  if (node->type == NODE_NAMED_ARG) {
    if (strcmp(node->value, "intensity") == 0) {
      if (strcmp(node->left->value, "MAX_FACTOR") == 0)
        current_intensity = 100;
      else if (node->left->type == NODE_LITERAL)
        current_intensity = atoi(node->left->value);

      // Apply intensity drop-off curve based on skill level: i = i * (s^2 /
      // 10000)
      double skill_factor =
          (double)current_skill_level * (double)current_skill_level / 10000.0;
      current_intensity = (int)((double)current_intensity * skill_factor);

      return "";
    }
    return manifest_expr(node->left);
  }

  return "";
}

char *manifest_gesture_actions(ASTNode *node) {
  if (!node)
    return "";
  char *current = manifest_expr(node);
  char *remaining = manifest_gesture_actions(node->next);
  if (strlen(current) == 0)
    return remaining;
  if (strlen(remaining) == 0)
    return current;
  char *buf = (char *)malloc(2048);
  sprintf(buf, "%s, while %s", current, remaining);
  return buf;
}

char *scan_for_gesture(ASTNode *node) {
  if (!node)
    return NULL;

  if (node->type == NODE_CALL && node->left &&
      node->left->type == NODE_MEMBER_ACCESS) {
    if (node->left->left && strcmp(node->left->left->value, "Trigger") == 0 &&
        strcmp(node->left->value, "gesture") == 0) {
      ASTNode *arg = node->right;
      while (arg) {
        if (arg->type == NODE_NAMED_ARG && strcmp(arg->value, "action") == 0) {
          return manifest_expr(arg->left);
        }
        arg = arg->next;
      }
    }
  }
  char *leftRes = scan_for_gesture(node->left);
  if (leftRes)
    return leftRes;
  return scan_for_gesture(node->right);
}

double scan_for_radius(ASTNode *node, const char *type) {
  if (!node)
    return 0;
  if (node->type == NODE_CALL) {
    char *func = manifest_expr(node->left);
    ASTNode *arg = node->right;
    while (arg) {
      if (arg->type == NODE_NAMED_ARG && strcmp(arg->value, "radius") == 0) {
        double val = 0;
        if (arg->left->type == NODE_BINOP) {
          val = (double)current_intensity * (double)current_skill_level / 100.0;
        } else if (arg->left && arg->left->type == NODE_LITERAL) {
          val = atof(arg->left->value);
        }

        if (val > 0) {
          if (strcmp(type, "sphere") == 0 &&
              (strstr(func, "sphere") || strstr(func, "fireball")))
            return (double)current_intensity / 50.0;
          if (strcmp(type, "targeting") == 0 && !strstr(func, "sphere") &&
              !strstr(func, "fireball"))
            return val;
        }
      }
      arg = arg->next;
    }
  }
  double left = scan_for_radius(node->left, type);
  if (left > 0)
    return left;
  return scan_for_radius(node->right, type);
}

void collect_info(ASTNode *node) {
  if (!node)
    return;

  if (node->type == NODE_INCANTATION)
    add_ritual(node->value);
  if (node->type == NODE_INSTANCE) {
    char *dist = NULL;
    ASTNode *arg = node->left->next;
    while (arg) {
      if (arg->type == NODE_NAMED_ARG && strcmp(arg->value, "distance") == 0)
        dist = arg->left->value;
      arg = arg->next;
    }
    add_instance(node->value, node->left->value, dist);
    ASTNode *d = node->left->next;
    while (d) {
      if (d->type == NODE_CALL && strcmp(d->value, "with_clothing") == 0 &&
          d->left)
        decorate_instance(node->value, d->left->value);
      d = d->next;
    }
  }
  if (node->type == NODE_IMBUE) {
    ASTNode *curr = node->right;
    char *required_gest = NULL;
    double t_radius = 0;
    double s_radius = 0;

    // Capture intensities and essences within this block
    collect_info(node->right);

    // Pre-pass to find requirements in the imbue block AFTER intensity drop-off
    ASTNode *scan = node->right;
    while (scan) {
      char *found_g = scan_for_gesture(scan);
      if (found_g)
        required_gest = found_g;

      double found_t = scan_for_radius(scan, "targeting");
      if (found_t > 0)
        t_radius = found_t;

      double found_s = scan_for_radius(scan, "sphere");
      if (found_s > 0)
        s_radius = found_s;

      scan = scan->next;
    }

    while (curr) {
      if (curr->type == NODE_FLOW) {
        char *flow_desc = (char *)malloc(1024);
        sprintf(flow_desc, "%s -> %s", manifest_expr(curr->left),
                manifest_expr(curr->right));
        char *essence = "arcane";
        int intensity = current_intensity;
        if (curr->left->type == NODE_MEMBER_ACCESS &&
            strcmp(curr->left->left->value, "Essence") == 0) {
          essence = curr->left->value;
        }
        add_enchantment(node->value, flow_desc, essence, intensity,
                        required_gest, t_radius, s_radius);
      }
      curr = curr->next;
    }
    collect_info(node->next);
    return;
  }
  if (node->type == NODE_LET) {
    collect_info(node->left); // Expression side
  }
  if (node->type == NODE_FLOW) {
    collect_info(node->left);
    collect_info(node->right);
  }
  if (node->type == NODE_GIVEN_BLOCK) {
    ASTNode *call = node->left;
    if (call && call->type == NODE_CALL) {
      ASTNode *arg = call->right;
      while (arg) {
        if (arg->type == NODE_NAMED_ARG &&
            strcmp(arg->value, "skill_level") == 0) {
          if (strcmp(arg->left->value, "MAX_FACTOR") == 0)
            current_skill_level = 100;
          else
            current_skill_level = atoi(arg->left->value);
        }
        arg = arg->next;
      }
    }
  }
  if (node->type == NODE_GESTURE_STMT) {
    char *gest = manifest_gesture_actions(node->left);
    char *buf = (char *)malloc(1024);
    // If it's a multi-part gesture, we might want to insert the object after
    // the first part if it's a contact action. However, for simplicity and
    // natural feel, we'll join them.
    if (strstr(gest, "while")) {
      // Split at "while" and insert "against [vessel]"
      char *prefix = strdup(gest);
      char *sep = strstr(prefix, ", while");
      if (sep) {
        *sep = '\0';
        char *suffix = sep + 2; // "while ..."
        sprintf(buf, "gesture by %s against %s, %s", prefix, node->value,
                suffix);
      } else {
        sprintf(buf, "gesture by %s against %s", gest, node->value);
      }
    } else {
      sprintf(buf, "gesture by %s against %s", gest, node->value);
    }
    last_gesture_prose = buf;
  }
  if (node->type == NODE_CALL && node->left &&
      node->left->type == NODE_MEMBER_ACCESS &&
      strcmp(node->left->left->value, "Essence") == 0) {
    int intensity = current_intensity;
    ASTNode *arg = node->right;
    while (arg) {
      if (arg->type == NODE_NAMED_ARG && strcmp(arg->value, "intensity") == 0) {
        if (arg->left->type == NODE_LITERAL)
          intensity = atoi(arg->left->value);
        else if (arg->left->type == NODE_BINOP)
          intensity =
              current_intensity / 2; // Simple approximation for fireball_staff
      }
      arg = arg->next;
    }
    add_active_essence(node->left->value, intensity);
  }

  if (node->type == NODE_CALL && node->left &&
      node->left->type == NODE_MEMBER_ACCESS &&
      strcmp(node->left->left->value, "Func") == 0 &&
      strcmp(node->left->value, "trigger_intensity") == 0) {
    manifest_expr(node); // Triggers intensity drop-off
  }

  if (node->type == NODE_CALL) {
    char *func = manifest_expr(node->left);
    ASTNode *arg = node->right;
    while (arg) {
      if (arg->type == NODE_NAMED_ARG && strcmp(arg->value, "radius") == 0) {
        double val = 0;
        if (arg->left->type == NODE_BINOP)
          val = (double)current_intensity * (double)current_skill_level / 100.0;
        else if (arg->left && arg->left->type == NODE_LITERAL)
          val = atof(arg->left->value);

        if (val > 0) {
          current_targeting_radius = val;
        }
      }
      arg = arg->next;
    }
    if (current_intensity > 0) {
      // Values are captured into Enchantment below
    }
    if (strstr(func, "sphere") || strstr(func, "fireball"))
      current_sphere_radius = (double)current_intensity / 50.0;
  }

  collect_info(node->left);
  collect_info(node->right);
  collect_info(node->next);
}

void execute_ritual(ASTNode *root) {
  find_initial_intensity(root);
  find_initial_skill_level(root);
  collect_info(root);
  printf("\n-------- Spells --------\n");
  printf("You've focused your will and incanted powerful spells:\n");
  Ritual *r = rituals;
  while (r) {
    printf("- %s\n", r->name);
    r = r->next;
  }
  printf("------------------------\n");
  printf("\n-------- Surroundings --------\n");
  Instance *i = world_instances;
  while (i) {
    const char *desc = lookup_lexicon(interface_lexicon, i->interface);
    if (strcmp(i->name, "ash_staff") == 0)
      printf("You see %s, an expertly carved %s.", i->name, desc);
    else if (i->decoration)
      printf("You see %s, %s, clad in %s.", i->name, desc, i->decoration);
    else
      printf("You see %s, %s.", i->name, desc);
    if (i->distance)
      printf(" It stands %sU away.", i->distance);
    if (i->hp > 0)
      printf(" (HP: %d)", i->hp);
    printf("\n");
    i = i->next;
  }
  printf("------------------------------\n");
  printf("\n-------- Enchantments --------\n");
  Enchantment *e_ptr = enchantments;
  while (e_ptr) {
    const char *desc = lookup_lexicon(interface_lexicon, "Staff");
    printf("The %s %s has been imbued with an enchantment:\n", e_ptr->vessel,
           desc);
    printf("- SYNERGY: %s\n", e_ptr->synergy);
    e_ptr = e_ptr->next;
  }
  printf("------------------------------\n");
  if (current_skill_level > 0)
    printf("\n**YOUR SKILL LEVEL IS %d**\n", current_skill_level);
  if (last_gesture_prose)
    printf("\nWith %d%% intensity, you %s.\n", current_intensity,
           last_gesture_prose);

  Enchantment *active_e = enchantments;
  while (active_e) {
    // Malfunction check
    if (active_e->required_gesture) {
      if (!last_gesture_prose ||
          strstr(last_gesture_prose, active_e->required_gesture) == NULL) {
        // Special case: if the required gesture has "against ," and the
        // performed has "against [vessel]" We handle this by stripping
        // "against" from the check if strstr fails, but only for validation
        // purposes.
        char *sanitized_required = strdup(active_e->required_gesture);
        char *comma = strstr(sanitized_required, ",");
        if (comma)
          *comma = '\0'; // Just check first part

        if (!last_gesture_prose ||
            strstr(last_gesture_prose, sanitized_required) == NULL) {
          printf("\n[MALFUNCTION] The enchantment on %s ripples violently!\n",
                 active_e->vessel);
          printf("The gesture mismatch HAS CAUSED THE RITUAL TO COLLAPSE.\n");
          active_e = active_e->next;
          continue;
        }
      }
    }

    if (active_e->targeting_radius > 0)
      printf("Ritual focus for '%s' extends up to %gU.\n", active_e->synergy,
             active_e->targeting_radius);

    Instance *target = world_instances;
    while (target) {
      if (strcmp(target->interface, "Enemy") == 0) {
        int target_match = is_combustible(target);

        int dist = target->distance ? atoi(target->distance) : 0;
        if (dist > active_e->targeting_radius) {
          printf("\nThe spell '%s' fails to reach %s. It stands %dU away, "
                 "but your focus only reached %gU.\n",
                 active_e->synergy, target->name, dist,
                 active_e->targeting_radius);
        } else if (!target_match) {
          printf("\nThe spell '%s' bypasses %s as it does not match the "
                 "ritual's required interface (Combustible).\n",
                 active_e->synergy, target->name);
        } else {
          printf("\n");
          if (active_e->sphere_radius > 0)
            printf("Upon impact with %s, the fireball detonates, expanding in "
                   "a sphere with radius %gU, centered on %s.\n",
                   target->name, active_e->sphere_radius, target->name);

          char consequences[2048] = "";
          int cons_count = 0;
          int total_damage = 0;

          ActiveEssence *ae = active_essences;
          while (ae) {
            char tech_key[128];
            sprintf(tech_key, "Essence.%s", ae->name);
            const char *tech_desc = lookup_lexicon(essence_lexicon, tech_key);
            const char *narrative = lookup_lexicon(essence_lexicon, ae->name);

            printf("The spell hits %s with ", target->name);
            printf(tech_desc, ae->intensity);
            printf("! You see %s.\n", narrative);

            if (strcmp(ae->name, "force") == 0) {
              printf("This focused burst of force propels the ritual forward, "
                     "enabling a direct vector strike against %s.\n",
                     target->name);
            }

            total_damage += ae->intensity;

            const char *c[2] = {
                lookup_consequence(ae->name, target->interface),
                target->decoration
                    ? lookup_consequence(ae->name, target->decoration)
                    : NULL};

            for (int k = 0; k < 2; k++) {
              if (c[k]) {
                if (cons_count > 0) {
                  size_t len = strlen(consequences);
                  if (len > 0 && consequences[len - 1] == '.') {
                    consequences[len - 1] = '\0';
                  }
                  strcat(consequences, " while ");
                  if (strncmp(c[k], "is ", 3) == 0) {
                    strcat(consequences, c[k] + 3);
                  } else {
                    strcat(consequences, c[k]);
                  }
                } else {
                  strcat(consequences, c[k]);
                }
                cons_count++;
              }
            }
            ae = ae->next;
          }

          target->hp -= total_damage;
          if (cons_count > 0) {
            printf("%s %s\n", target->name, consequences);
          }

          if (target->hp > 0) {
            printf("%s is wounded, but still stands (HP: %d).\n", target->name,
                   target->hp);
          } else if (target->hp + total_damage > 0) {
            if (cons_count == 0) {
              printf("%s has been defeated!\n", target->name);
            }
          }
        }
      }
      target = target->next;
    }
    active_e = active_e->next;
  }
}

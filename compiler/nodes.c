#include "nodes.h"
#include <stdlib.h>
#include <string.h>

static Interface *registry = NULL;
static int registry_size = 0;

void init_reality_registry() {
  registry_size = 10;
  registry = (Interface *)malloc(sizeof(Interface) * registry_size);

  // Define Combustible
  registry[0].interface_name = strdup("Combustible");
  registry[0].port_count = 3;
  registry[0].ports = (Port *)malloc(sizeof(Port) * 3);
  registry[0].ports[0].name = strdup("reaction");
  registry[0].ports[0].type = PORT_INPUT;
  registry[0].ports[1].name = strdup("temperature");
  registry[0].ports[1].type = PORT_OUTPUT;
  registry[0].ports[2].name = strdup("momentum");
  registry[0].ports[2].type = PORT_INPUT;

  // Define Sophont
  registry[1].interface_name = strdup("Sophont");
  registry[1].port_count = 1;
  registry[1].ports = (Port *)malloc(sizeof(Port) * 1);
  registry[1].ports[0].name = strdup("will");
  registry[1].ports[0].type = PORT_OUTPUT;

  // Define Vessel
  registry[2].interface_name = strdup("Vessel");
  registry[2].port_count = 2;
  registry[2].ports = (Port *)malloc(sizeof(Port) * 2);
  registry[2].ports[0].name = strdup("glow");
  registry[2].ports[0].type = PORT_INPUT;
  registry[2].ports[1].name = strdup("charge");
  registry[2].ports[1].type = PORT_OUTPUT;

  // Define Staff
  registry[3].interface_name = strdup("Staff");
  registry[3].port_count = 2;
  registry[3].ports = (Port *)malloc(sizeof(Port) * 2);
  registry[3].ports[0].name = strdup("focus");
  registry[3].ports[0].type = PORT_INPUT;
  registry[3].ports[1].name = strdup("resonance");
  registry[3].ports[1].type = PORT_OUTPUT;

  // Define World
  registry[4].interface_name = strdup("World");
  registry[4].port_count = 3;
  registry[4].ports = (Port *)malloc(sizeof(Port) * 3);
  registry[4].ports[0].name = strdup("ping");
  registry[4].ports[0].type = PORT_INPUT;
  registry[4].ports[1].name = strdup("sphere");
  registry[4].ports[1].type = PORT_INPUT;
  registry[4].ports[2].name = strdup("line");
  registry[4].ports[2].type = PORT_INPUT;

  // Define Func
  registry[5].interface_name = strdup("Func");
  registry[5].port_count = 3;
  registry[5].ports = (Port *)malloc(sizeof(Port) * 3);
  registry[5].ports[0].name = strdup("filter");
  registry[5].ports[0].type = PORT_INPUT;
  registry[5].ports[1].name = strdup("first");
  registry[5].ports[1].type = PORT_INPUT;
  registry[5].ports[2].name = strdup("trigger_intensity");
  registry[5].ports[2].type = PORT_INPUT;

  // Define User
  registry[6].interface_name = strdup("User");
  registry[6].port_count = 2;
  registry[6].ports = (Port *)malloc(sizeof(Port) * 2);
  registry[6].ports[0].name = strdup("skill_level");
  registry[6].ports[0].type = PORT_OUTPUT;
  registry[6].ports[1].name = strdup("simulate");
  registry[6].ports[1].type = PORT_INPUT;

  // Define Enemy
  registry[7].interface_name = strdup("Enemy");
  registry[7].port_count = 1;
  registry[7].ports = (Port *)malloc(sizeof(Port) * 1);
  registry[7].ports[0].name = strdup("distance");
  registry[7].ports[0].type = PORT_OUTPUT;

  // Define Container
  registry[8].interface_name = strdup("Container");
  registry[8].port_count = 1;
  registry[8].ports = (Port *)malloc(sizeof(Port) * 1);
  registry[8].ports[0].name = strdup("contents");
  registry[8].ports[0].type = PORT_OUTPUT;

  // Define Vector
  registry[9].interface_name = strdup("Vector");
  registry[9].port_count = 1;
  registry[9].ports = (Port *)malloc(sizeof(Port) * 1);
  registry[9].ports[0].name = strdup("straight");
  registry[9].ports[0].type = PORT_INPUT;
}

bool interface_has_port(const char *interface_name, const char *port_name,
                        PortType type) {
  if (!registry)
    init_reality_registry();
  for (int i = 0; i < registry_size; i++) {
    if (strcmp(registry[i].interface_name, interface_name) == 0) {
      for (int j = 0; j < registry[i].port_count; j++) {
        if (strcmp(registry[i].ports[j].name, port_name) == 0 &&
            registry[i].ports[j].type == type) {
          return true;
        }
      }
    }
  }
  return false;
}

bool is_valid_interface(const char *name) {
  if (!registry)
    init_reality_registry();
  for (int i = 0; i < registry_size; i++) {
    if (strcmp(registry[i].interface_name, name) == 0)
      return true;
  }
  // Simple types
  if (strcmp(name, "Factor") == 0)
    return true;
  if (strcmp(name, "Distance") == 0)
    return true;
  if (strcmp(name, "Type") == 0)
    return true;
  return true; // Flexible for now
}

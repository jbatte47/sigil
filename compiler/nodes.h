#ifndef SIGIL_NODES_H
#define SIGIL_NODES_H

#include <stdbool.h>

typedef enum { PORT_INPUT, PORT_OUTPUT } PortType;

typedef struct {
  char *name;
  PortType type;
} Port;

typedef struct {
  char *interface_name;
  Port *ports;
  int port_count;
} Interface;

void init_reality_registry();
bool interface_has_port(const char *interface_name, const char *port_name,
                        PortType type);
bool is_valid_interface(const char *name);

#endif

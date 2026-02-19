#ifndef SIGIL_LEXICON_H
#define SIGIL_LEXICON_H

#include <string.h>

typedef struct {
  char *technical;
  char *narrative;
} LexiconEntry;

static LexiconEntry essence_lexicon[] = {
    {"Heat", "shimmering waves of intense heat"},
    {"Force", "a crushing wall of kinetic pressure"},
    {"Ping", "a subtle ripple in the local aether"},
    {NULL, NULL}};

static LexiconEntry interface_lexicon[] = {
    {"Combustible", "vulnerable to the touch of flame"},
    {"Staff", "an expertly carved focal point"},
    {"Enemy", "a looming threat"},
    {"Surface", "a cold, unyielding boundary"},
    {NULL, NULL}};

static LexiconEntry gesture_lexicon[] = {
    {"sweep", "sweeping the vessel in a wide, commanding arc"},
    {"long_press", "pressing firmly against the surface with focused intent"},
    {"thumb", "your thumb"},
    {"right", "your right hand"},
    {NULL, NULL}};

static char *lookup_lexicon(LexiconEntry *entries, const char *technical) {
  for (int i = 0; entries[i].technical != NULL; i++) {
    if (strcmp(entries[i].technical, technical) == 0) {
      return entries[i].narrative;
    }
  }
  return (char *)technical;
}

#endif

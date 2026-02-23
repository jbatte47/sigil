#ifndef SIGIL_LEXICON_H
#define SIGIL_LEXICON_H

#include <string.h>

typedef struct {
  const char *technical;
  const char *narrative;
} LexiconEntry;

typedef struct {
  const char *essence;
  const char *decoration;
  const char *consequence;
} ConsequenceEntry;

static LexiconEntry essence_lexicon[] = {
    {"heat", "shimmering waves of intense heat"},
    {"force", "a crushing wall of kinetic pressure"},
    {"Essence.heat", "%d%% intensity"},
    {"Essence.force", "%d%% force"},
    {NULL, NULL}};

static LexiconEntry interface_lexicon[] = {
    {"Combustible", "vulnerable to the touch of flame"},
    {"Staff", "Magical Staff"},
    {"Enemy", "a savage Enemy"},
    {"Surface", "a cold, unyielding boundary"},
    {NULL, NULL}};

static LexiconEntry gesture_lexicon[] = {
    {"Action.sweep", "sweeping it in a broad arc"},
    {"Action.long_press(hand=Right, finger=Thumb)",
     "long pressing the thumb of your right hand"},
    {"Thumb", "your thumb"},
    {"Right", "your right hand"},
    {NULL, NULL}};

// The Consequence Engine mappings
static ConsequenceEntry consequence_lexicon[] = {
    {"heat", "rags",
     "is engulfed as its clothing ignites, enveloping it in flames. It "
     "crumples to the floor in an ashen heap"},
    {"force", "Enemy", "is sent tumbling backward by the invisible pressure"},
    {NULL, NULL, NULL}};

static const char *lookup_lexicon(LexiconEntry *entries,
                                  const char *technical) {
  for (int i = 0; entries[i].technical != NULL; i++) {
    if (strcmp(entries[i].technical, technical) == 0) {
      return entries[i].narrative;
    }
  }
  return technical;
}

static const char *lookup_consequence(const char *essence,
                                      const char *decoration) {
  for (int i = 0; consequence_lexicon[i].essence != NULL; i++) {
    if (strstr(essence, consequence_lexicon[i].essence) &&
        strstr(decoration, consequence_lexicon[i].decoration)) {
      return consequence_lexicon[i].consequence;
    }
  }
  return NULL;
}

#endif

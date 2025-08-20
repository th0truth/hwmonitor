#pragma once

#include <inttypes.h>
#include "platform.h"

#if defined(HWMONITOR_UNIX)
typedef struct {
  char *NAME;
  int16_t VERSION_ID;
  char *RELEASE_TYPE;
  char *ID;
  char *DE;
  char *DE_ID;
} OS;

OS *getOSinfo();
void free_os(OS *os);
#endif
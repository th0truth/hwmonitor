#pragma once

#include "platform.h"

#if defined(HWMONITOR_UNIX)
typedef struct {
  char *name;
  char *family;
  char *serial;
  char *sku;
} MAINBOARD;

MAINBOARD *getMainboardInfo();
void free_mainboard(MAINBOARD *mb);
#endif
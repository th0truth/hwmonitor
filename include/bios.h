#pragma once

#include "platform.h"

#if defined(HWMONITOR_UNIX)
typedef struct {
  char *vendor;
  char *version;
  char *date;
} BIOS;

BIOS *getBIOSinfo();
void free_bios(BIOS *bios);
#endif
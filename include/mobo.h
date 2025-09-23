#pragma once

#include "platform.h"

#if defined(HWMONITOR_UNIX)
typedef struct {
  char *manufacturer;
  char *model;
  char *version;
} MOBO;

typedef struct {
  char *vendor;
  char *version;
  char *date;
} BIOS;

MOBO *getMOBOinfo();
BIOS *getBIOSinfo();
void free_mobo(MOBO *mobo);
#endif
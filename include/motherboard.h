#pragma once

#include "platform.h"

#if defined(HWMONITOR_UNIX)
typedef struct {
  char *manufacturer;
  char *model;
  char *version;
} Motherboard;

Motherboard *getMotherboardInfo();
void free_motherboard(Motherboard *mobo);
#endif
#pragma once

#include "platform.h"

#if defined(HWMONITOR_UNIX)
typedef struct {
  size_t total;
  size_t free;
  size_t available;
  size_t buffers;
  size_t cached;
  size_t swap_total;
  size_t swap_free;
  size_t Zswap;
  size_t Zswapped;
  size_t dirty;
  size_t perCpu;
} MEM;

MEM *getMEMinfo();
#endif
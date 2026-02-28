#pragma once

#include <stdint.h>
#include <cJSON.h>

typedef struct {
  uint64_t total;
  uint64_t free;
  uint64_t available;
  uint64_t buffers;
  uint64_t cached;
  uint64_t swap_total;
  uint64_t swap_free;
  uint64_t zswap;
  uint64_t zswapped;
  uint64_t dirty;
  uint64_t per_cpu;
} RAM;

cJSON* ram_to_json_obj(const RAM* ram);
RAM* ram_get_info();
void free_ram(RAM* ram);

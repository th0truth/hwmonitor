#pragma once

#include <stdint.h>
#include <cJSON.h>

typedef struct
{
  char* vendor_id;
  char* model_name;
  char* flags;
  char* arch;
  float max_MHz;
  float min_MHz;
  float curr_temp;
  uint16_t cpu_family;
  uint16_t model;
  uint16_t stepping;
  uint16_t total_cores;
  uint16_t total_threads;
  uint16_t online_cores;
} CPU;

cJSON* cpu_to_json_obj(const CPU* cpu);
CPU* cpu_get_info();
void free_cpu(CPU* cpu);

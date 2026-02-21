#pragma once

typedef struct
{
  char* vendor_id;
  char* model_name;
  char* flags;
  char* arch;
  char* online;
  float max_MHz;
  float min_MHz;
  float curr_temp;
  uint16_t cpu_family;
  uint16_t model;
  uint16_t stepping;
  uint16_t total_cores;
  uint16_t total_threads;
  uint16_t processors;
} CPU;

CPU* cpu_get_info();

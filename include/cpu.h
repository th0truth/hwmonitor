#pragma once

#include "platform.h"

#if defined(HWMONITOR_UNIX)
#define BUFF_SIZE 128

typedef struct {
  char *arch;
  char *vendor_id;
  char *model_name;
  char *flags;
  char *online;
  float max_MHz;
  float min_MHz;
  float curr_temp;
  unsigned cpu_family;
  unsigned model;
  unsigned stepping;
  unsigned total_cores;
  unsigned total_threads;
  uint16_t processors;
} CPU;

CPU *getCPUinfo();
void free_cpu(CPU *cpu);
#endif
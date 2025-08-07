#include <stdint.h>
#pragma once

#ifndef CPU_H
#define CPU_H

#define BUFF_SIZE 128

typedef struct {
  char *vendor_id;
  unsigned cpu_family;
  unsigned model;
  char *model_name;
  unsigned stepping;
  unsigned total_cores;
  unsigned total_threads;
  int64_t processors;
  float max_MHz;
  float min_MHz;
  float curr_temp;
  char *flags;
} CPU;

CPU *getCPUinfo();
#endif
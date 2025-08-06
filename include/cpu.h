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
  char *flags;
} CPU;

CPU *getCPUspecs();
int64_t getCPUCoreMaxFreq_MHz(unsigned core_id);
int64_t getCPUCoreMinFreq_MHz(unsigned core_id);
int64_t getCPUCoreRegularFreq_MHz(unsigned core_id);
float getCPUCurrentTemp_Celsius();
#endif
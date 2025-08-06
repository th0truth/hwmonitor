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
  char *flags;
} CPU;

CPU *getCPUinfo();
int64_t  getCPUCoreMaxFreq_MHz(unsigned short core_id);
int64_t  getCPUCoreMinFreq_MHz(unsigned short core_id);
int64_t  getCPUCoreRegFreq_MHz(unsigned short core_id);
uint64_t getCPUMaxFreq_MHz(unsigned short t_cores);
uint64_t getCPUMinFreq_MHz(unsigned short t_cores);
float   getCPUCurrTemp_Celsius();
#endif
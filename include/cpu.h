#pragma once

#ifndef CPU_H
#define CPU_H

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
#endif
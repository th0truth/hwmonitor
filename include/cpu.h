#pragma once

#ifndef CPU_H
#define CPU_H

typedef struct {
  size_t total_cores;
  size_t total_threads;
  char *model_name;
} CPU;

CPU *getCPUspecs();
#endif
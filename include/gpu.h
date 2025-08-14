#pragma once

#ifndef GPU_H
#define GPU_H

#define BUFF_SIZE 128

#define NVIDIA "0x10de"
#define INTEL "0x8086"
#define AMD "0x1002"

typedef struct {
  char *vendor;
  char *driver;
  char *model;
} GPU;

GPU *getGPUinfo();
void free_gpu(GPU *gpu);    
#endif
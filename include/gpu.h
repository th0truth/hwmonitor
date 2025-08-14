#pragma once

#ifndef GPU_H
#define GPU_H

#define BUFF_SIZE 128

#define NVIDIA "0x10de"
#define INTEL "0x8086"
#define AMD "0x1002"

typedef struct {
  char *vendor;
  char *device_id;
  char *subsys_vendor;
  char *subsys_device;
  char *driver;
  char *model;
  char *pci_id;
  char *pci_subsys;
  char *pci_slot_name;
} GPU;

GPU *getGPUinfo();
void free_gpu(GPU *gpu);    
#endif
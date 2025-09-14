#pragma once

#include <inttypes.h>
#include "platform.h"

#if defined(HWMONITOR_UNIX)
#define BUFF_SIZE 128
#define MAX_GPUS 8

// GPUs vendor id
#define NVIDIA "0x10de"
#define INTEL "0x8086"
#define AMD "0x1002"

typedef struct {
  char *model;
  uint16_t IRQ;
  char *UUID;
  char *vbios;
  char *bus_type;
  char *firmware;
  char *kernel_version;
  char *vendor;
  char *device_id;
  char *subsys_vendor;
  char *subsys_device;
  char *driver;
  char *pci_id;
  char *pci_subsys;
  char *pci_slot_name;
} GPU;

int getGPUinfo(GPU **gpus);
void free_gpu(GPU *gpu);
#endif
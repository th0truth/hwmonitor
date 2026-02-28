/**
 * @file gpu.h
 * @brief Header file for gpu module.
 */

#pragma once

#include <inttypes.h>
#include <cJSON.h>

#define MAX_GPUS 16

// GPU Vendor IDs (PCI hex codes)
#define PCI_VENDOR_NVIDIA "0x10de"
#define PCI_VENDOR_INTEL  "0x8086"
#define PCI_VENDOR_AMD    "0x1002"

typedef struct {
  char* model;
  char* irq;
  char* uuid;
  char* vbios;
  char* bus_type;
  char* firmware;
  char* kernel_version;
  char* vendor;
  char* device_id;
  char* subsys_vendor;
  char* subsys_device;
  char* driver;
  char* pci_id;
  char* pci_subsys;
  char* pci_slot_name;
} GPU;

/**
 * Discovers all GPUs in the system via sysfs.
 * Returns a dynamically allocated array of GPU pointers.
 * 'count' is updated with the number of GPUs found.
 */
GPU** gpu_get_all(int* count);

/**
 * Frees a single GPU structure and all its internal strings.
 */
void free_gpu(GPU* gpu);

/**
 * Frees an array of GPU pointers and the array itself.
 */
void free_gpus(GPU** gpus, int count);

/**
 * Converts a GPU structure to a cJSON object.
 */
cJSON* gpu_to_json_obj(const GPU* gpu);

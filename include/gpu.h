#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <inttypes.h>
#include <cJSON.h>

#define MAX_GPUS 16

#define PCI_VENDOR_NVIDIA "0x10de"
#define PCI_VENDOR_INTEL  "0x8086"
#define PCI_VENDOR_AMD    "0x1002"

typedef struct {
    char *model;
    char *irq;
    char *uuid;
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

GPU **gpu_get_all(int *count);
void free_gpu(GPU *gpu);
void free_gpus(GPU **gpus, int count);
cJSON *gpu_to_json_obj(const GPU *gpu);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
 * \file gpu.h
 * \brief Header file for gpu module.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <inttypes.h>
#include <cJSON.h>

#define MAX_GPUS 16

/* GPU Vendor IDs (PCI hex codes) */
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

/**
 * \brief Discovers all GPUs in the system via sysfs.
 * \param[out] count Pointer to an integer where the number of GPUs will be stored.
 * \return A dynamically allocated array of GPU pointers, or NULL on failure.
 */
GPU **gpu_get_all(int *count);

/**
 * \brief Frees a single GPU structure and all its internal strings.
 * \param[in] gpu Pointer to the GPU structure to free.
 */
void free_gpu(GPU *gpu);

/**
 * \brief Frees an array of GPU pointers and the array itself.
 * \param[in] gpus Array of GPU pointers to free.
 * \param[in] count Number of GPUs in the array.
 */
void free_gpus(GPU **gpus, int count);

/**
 * \brief Converts a GPU structure to a cJSON object.
 * \param[in] gpu Pointer to the GPU structure.
 * \return Pointer to a cJSON object (caller must delete).
 */
cJSON *gpu_to_json_obj(const GPU *gpu);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "utils.h"
#include "gpu.h"

GPU *getGPUinfo()
{
  // Allocate memory
  GPU *gpu = malloc(sizeof *gpu);
  if (gpu == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    return NULL;
  }

  char buff[BUFF_SIZE];
  for (int i = 0; ; ++i) {
    // Get the GPU Vendor ID
    snprintf(buff, BUFF_SIZE, "/sys/class/drm/card%d/device/vendor", i);
    gpu->vendor = read_file(buff, "\n", 0) ;
    if (gpu->vendor == NULL) {
      free(gpu);
      return NULL;
    }
    gpu->vendor[strcspn(gpu->vendor, "\xff")] = '\0';
    
    snprintf(buff, BUFF_SIZE, "/sys/class/drm/card%d/device/device", i);
    gpu->device_id = read_file(buff, "\n", 0);

    snprintf(buff, BUFF_SIZE, "/sys/class/drm/card%d/device/subsystem_device", i);
    gpu->subsys_device = read_file(buff, "\n", 0);

    snprintf(buff, BUFF_SIZE, "/sys/class/drm/card%d/device/subsystem_vendor", i);
    gpu->subsys_vendor = read_file(buff, "\n", 0);

    snprintf(buff, BUFF_SIZE, "/sys/class/drm/card%d/device/uevent", i);
    char *uevent = read_file(buff, "=", 0);
    if (uevent == NULL) {
      free(gpu);
      return NULL;
    }

    // Extract GPU info from uevent
    gpu->driver        = findstr(uevent, "DRIVER", "\n");
    gpu->pci_id        = findstr(uevent, "PCI_ID", "\n");
    gpu->pci_subsys    = findstr(uevent, "PCI_SUBSYS_ID", "\n");
    gpu->pci_slot_name = findstr(uevent, "PCI_SLOT_NAME", "\n");
    
    free(uevent);
    
    if (strcmp(gpu->vendor, NVIDIA) == 0) {
      snprintf(buff, BUFF_SIZE, "/proc/driver/nvidia/gpus/%s/information", gpu->pci_slot_name);
      
      // Get details about an NVIDIA GPU
      char *n_info = read_file(buff, ":\t", 0);
      if (n_info == NULL) {
        return NULL;
      }
      
      gpu->model = findstr(n_info, "Model  ", "\n");
      
      return gpu;
    } else if (strcmp(gpu->vendor, INTEL) == 0) {
      FILE *fp = popen("lspci -nnk | grep i915 -B2", "r");
      if (fp == NULL) {
        return NULL;
      }
      
      // Extract Intel CPU model name
      while (fgets(buff, BUFF_SIZE, fp) != NULL) {
        char *model = findstr(buff, "VGA compatible controller [0300]:", "\0");
        if (model) {
          gpu->model = model;
        }
      }
  
      return gpu;
    }
    free(gpu->vendor);
  }
}

void free_gpu(GPU *gpu)
{
  if (!gpu) return;
  free(gpu->vendor);
  free(gpu->device_id);
  free(gpu->subsys_vendor);
  free(gpu->subsys_device);
  free(gpu->driver);
  free(gpu->model);
  free(gpu->pci_id);
  free(gpu->pci_subsys);
  free(gpu->pci_slot_name);
  free(gpu);
}
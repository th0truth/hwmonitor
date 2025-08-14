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

  // Gather specific GPU info
  char buff[BUFF_SIZE];
  for (int i = 0; ; ++i) {
    // Get a GPU Vendor ID
    snprintf(buff, BUFF_SIZE, "/sys/class/drm/card%d/device/vendor", i);
    char *vendor = read_file(buff, "\n", 0);
    if (vendor == NULL) {
      free(gpu);
      return NULL;
    }
    vendor[strcspn(vendor, "\xff")] = '\0';
    
    snprintf(buff, BUFF_SIZE, "/sys/class/drm/card%d/device/uevent", i);
    char *uevent = read_file(buff, NULL, 0);
    if (uevent == NULL) {
      free(gpu);
      return NULL;
    }
  
    if (strcmp(vendor, NVIDIA) == 0) {
      // Extract PCI_SLOT from driver detal
      char *PCI_SLOT = findstr(uevent, "PCI_SLOT_NAME=", "\n");
      if (PCI_SLOT == NULL) {
        free(gpu);
        return NULL;
      }
      
      snprintf(buff, BUFF_SIZE, "/proc/driver/nvidia/gpus/%s/information", PCI_SLOT);
      free(PCI_SLOT);
      
      // Get details about an NVIDIA GPU
      char *n_info = read_file(buff, ":\t", 0);
      if (n_info == NULL) {
        return NULL;
      }
      
      gpu->vendor = vendor;
      gpu->driver = findstr(uevent, "DRIVER=", "\n");
      gpu->model = findstr(n_info, "Model  ", "\n");
      
      free(uevent);
      free(n_info);

      return gpu;
  }
    free(vendor);
  }
}

void free_gpu(GPU *gpu)
{
  if (!gpu) return;
  free(gpu->vendor);
  free(gpu->driver);
  free(gpu->model);
  free(gpu);
}
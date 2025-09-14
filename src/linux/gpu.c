#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "utils.h"
#include "gpu.h"

int getGPUinfo(GPU **gpus)
{
  char buff[BUFF_SIZE];
  int count = 0;

  for (int i = 0; i < MAX_GPUS; ++i) {
    GPU *gpu = malloc(sizeof *gpu);
    if (gpu == NULL) {
      fprintf(stderr, "Memory allocation failed.");
      break;
    }
    memset(gpu, 0, sizeof *gpu);

      // Get the GPU Vendor ID
    snprintf(buff, BUFF_SIZE, "/sys/class/drm/card%d/device/vendor", i);
    gpu->vendor = read_file(buff, "\n", 0) ;
    if (gpu->vendor == NULL) {
      free_gpu(gpu);
      continue;
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
      free_gpu(gpu);
      continue;
    }

    // Extract GPU info from uevent
    gpu->driver        = findstr(uevent, "DRIVER", "\n");
    gpu->pci_id        = findstr(uevent, "PCI_ID", "\n");
    gpu->pci_subsys    = findstr(uevent, "PCI_SUBSYS_ID", "\n");
    gpu->pci_slot_name = findstr(uevent, "PCI_SLOT_NAME", "\n");
    free(uevent);
      
    // Vendor-specific handling
    if (strcmp(gpu->vendor, NVIDIA) == 0) {
      // Get details about an NVIDIA GPU
      snprintf(buff, BUFF_SIZE, "/proc/driver/nvidia/gpus/%s/information", gpu->pci_slot_name);
      char *n_info = read_file(buff, "\t", 0);
      if (n_info) {
        gpu->model = findstr(n_info, "Model:  ", "\n");
        gpu->IRQ = atoi(findstr(n_info, "IRQ: ", "\n"));
        gpu->UUID = findstr(n_info, "UUID:  ", "\n");
        gpu->vbios = findstr(n_info, "Video BIOS:  ", "\n");
        gpu->bus_type = findstr(n_info, "Bus Type:  ", "\n");
        gpu->firmware = findstr(n_info, "GPU Firmware:  ", "\n");
        free(n_info);
      }
      
      // Get version of NVIDIA kernel driver module
      char *k_version = read_file("/proc/driver/nvidia/version", ":", 0);
      if (k_version) {
        gpu->kernel_version = trim(findstr(k_version, "NVRM version", "\n"));
        free(k_version);
      }
      
    } else if (strcmp(gpu->vendor, INTEL) == 0) {
      FILE *fp = popen("lspci -nnk | grep i915 -B2", "r");
      if (fp) {
        while (fgets(buff, BUFF_SIZE, fp) != NULL) {
          char *model = findstr(buff, "VGA compatible controller [0300]:", "\0");
          if (model) {
            gpu->model = model;
            break;
          }
        }
        pclose(fp);
      }
    }
    gpus[count++] = gpu;
  }

  return count;
}

void free_gpu(GPU *gpu)
{
  if (!gpu) return;
  free(gpu->model);
  free(gpu->UUID);
  free(gpu->vbios);
  free(gpu->bus_type);
  free(gpu->firmware);
  free(gpu->kernel_version);
  free(gpu->vendor);
  free(gpu->device_id);
  free(gpu->subsys_vendor);
  free(gpu->subsys_device);
  free(gpu->driver);
  free(gpu->pci_id);
  free(gpu->pci_subsys);
  free(gpu->pci_slot_name);
  free(gpu);
}
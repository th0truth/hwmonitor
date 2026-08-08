#include "base.h"
#include "file.h"
#include "io.h"
#include "gpu.h"
#include <dirent.h>

static void
gpu_handle_nvidia(GPU *gpu)
{
  char buffer[BUFFER_SIZE];
  /* NVIDIA driver exposes detailed information via procfs */
  snprintf(buffer, sizeof(buffer), "/proc/driver/nvidia/gpus/%s/information", gpu->pci_slot_name);

  char *n_info = file_read_stripped(buffer, "\t", false);
  if (n_info != NULL) {
    gpu->model    = str_find_value(n_info, "Model:  ", "\n");
    gpu->irq      = str_find_value(n_info, "IRQ:   ", "\n");
    gpu->uuid     = str_find_value(n_info, "UUID:  ", "\n");
    gpu->vbios    = str_find_value(n_info, "Video BIOS:  ", "\n");
    gpu->bus_type = str_find_value(n_info, "Bus Type:  ", "\n");
    gpu->firmware = str_find_value(n_info, "GPU Firmware:  ", "\n");
    free(n_info);
  }

  /* Retrieve global NVIDIA kernel module version */
  char *k_version = file_read_stripped("/proc/driver/nvidia/version", ":", false);
  if (k_version != NULL) {
    gpu->kernel_version = str_find_value(k_version, "NVRM version", "\n");
    free(k_version);
  }
}

static GPU *
gpu_parse_sysfs(const char *card_name)
{
  char buffer[BUFFER_SIZE];
  /* Read PCI Vendor ID to verify hardware presence */
  snprintf(buffer, sizeof(buffer), "/sys/class/drm/%s/device/vendor", card_name);

  char *vendor = file_read_stripped(buffer, "\n", false);
  if (vendor == NULL) {
    return NULL;
  }

  GPU *gpu = calloc(1, sizeof(*gpu));
  if (gpu == NULL) {
    free(vendor);
    return NULL;
  }
  gpu->vendor = vendor;

  /* Basic PCI identifiers */
  snprintf(buffer, sizeof(buffer), "/sys/class/drm/%s/device/device", card_name);
  gpu->device_id = file_read_stripped(buffer, "\n", false);

  snprintf(buffer, sizeof(buffer), "/sys/class/drm/%s/device/subsystem_device", card_name);
  gpu->subsys_device = file_read_stripped(buffer, "\n", false);

  snprintf(buffer, sizeof(buffer), "/sys/class/drm/%s/device/subsystem_vendor", card_name);
  gpu->subsys_vendor = file_read_stripped(buffer, "\n", false);

  /* Parse uevent file for driver and slot information */
  snprintf(buffer, sizeof(buffer), "/sys/class/drm/%s/device/uevent", card_name);
  char *uevent = file_read_stripped(buffer, "=", false);
  if (uevent != NULL) {
    gpu->driver        = str_find_value(uevent, "DRIVER", "\n");
    gpu->pci_id        = str_find_value(uevent, "PCI_ID", "\n");
    gpu->pci_subsys    = str_find_value(uevent, "PCI_SUBSYS_ID", "\n");
    gpu->pci_slot_name = str_find_value(uevent, "PCI_SLOT_NAME", "\n");
    free(uevent);
  }

  /* Check for vendor-specific extended data (e.g., NVIDIA) */
  if (gpu->vendor != NULL && strcmp(gpu->vendor, PCI_VENDOR_NVIDIA) == 0 && gpu->pci_slot_name != NULL) {
    gpu_handle_nvidia(gpu);
  }

  return gpu;
}

GPU **
gpu_get_all(int *count)
{
  *count = 0;
  DIR *dir = opendir("/sys/class/drm");
  if (dir == NULL) {
    return NULL;
  }

  GPU **list = calloc(MAX_GPUS, sizeof(GPU *));
  if (list == NULL) {
    closedir(dir);
    return NULL;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL && *count < MAX_GPUS) {
    /* Only process directories named "cardX" where X is a digit (ignore renderD nodes) */
    if (strncmp(entry->d_name, "card", 4) == 0 && isdigit((unsigned char)entry->d_name[4])) {
      GPU *gpu = gpu_parse_sysfs(entry->d_name);
      if (gpu != NULL) {
        list[(*count)++] = gpu;
      }
    }
  }

  closedir(dir);
  return list;
}

void
free_gpu(GPU *gpu)
{
  if (gpu == NULL) {
    return;
  }
  free(gpu->model);
  free(gpu->irq);
  free(gpu->uuid);
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

void
free_gpus(GPU **gpus, int count)
{
  if (gpus == NULL) {
    return;
  }
  for (int i = 0; i < count; ++i) {
    free_gpu(gpus[i]);
  }
  free(gpus);
}

cJSON *
gpu_to_json_obj(const GPU *gpu)
{
  cJSON *obj = cJSON_CreateObject();
  if (gpu == NULL) {
    return obj;
  }

  cJSON_AddStringToObject(obj, "vendor", STR_OR_UNK(gpu->vendor));
  cJSON_AddStringToObject(obj, "model", STR_OR_UNK(gpu->model));
  cJSON_AddStringToObject(obj, "driver", STR_OR_UNK(gpu->driver));
  cJSON_AddStringToObject(obj, "pci_id", STR_OR_UNK(gpu->pci_id));
  cJSON_AddStringToObject(obj, "kernel_version", STR_OR_UNK(gpu->kernel_version));
  cJSON_AddStringToObject(obj, "uuid", STR_OR_UNK(gpu->uuid));

  return obj;
}

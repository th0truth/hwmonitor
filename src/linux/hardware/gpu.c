#include "base.h"
#include "file.h"
#include "io.h"
#include "gpu.h"

static void
gpu_handle_nvidia(GPU *gpu)
{
    /* NVIDIA driver exposes detailed information via procfs */
    char *n_info = sysfs_read_attr_fmt("\t", "/proc/driver/nvidia/gpus/%s/information", gpu->pci_slot_name);
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
    /* Read PCI Vendor ID to verify hardware presence */
    char *vendor = sysfs_read_attr_fmt("\n", "/sys/class/drm/%s/device/vendor", card_name);
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
    gpu->device_id     = sysfs_read_attr_fmt("\n", "/sys/class/drm/%s/device/device", card_name);
    gpu->subsys_device = sysfs_read_attr_fmt("\n", "/sys/class/drm/%s/device/subsystem_device", card_name);
    gpu->subsys_vendor = sysfs_read_attr_fmt("\n", "/sys/class/drm/%s/device/subsystem_vendor", card_name);

    /* Parse uevent file for driver and slot information */
    char *uevent = sysfs_read_attr_fmt("=", "/sys/class/drm/%s/device/uevent", card_name);
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

static void *
gpu_parse_entry(const char *name)
{
    if (strncmp(name, "card", 4) == 0 && isdigit((unsigned char)name[4])) {
        return (void *)gpu_parse_sysfs(name);
    }

    return NULL;
}

GPU **
gpu_get_all(int *count)
{
    return (GPU **)sysfs_enumerate("/sys/class/drm", gpu_parse_entry, MAX_GPUS, count);
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

DEFINE_FREE_ARRAY(free_gpus, GPU, free_gpu)

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

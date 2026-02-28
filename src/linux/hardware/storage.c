/**
 * @file storage.c
 * @brief Hardware discovery and parsing logic for system block devices.
 */

#include "base.h"
#include "file.h"
#include "io.h"
#include "storage.h"
#include <dirent.h>

#define MAX_STORAGES 64

/**
 * Parses generic block device information from sysfs.
 * @param block_name The block device name (e.g., "sda", "nvme0n1").
 * @return A newly allocated STORAGE struct, or NULL on failure.
 */

static void trim_trailing_spaces(char* str) {
  if (!str) return;
  int len = strlen(str);
  while (len > 0 && isspace((unsigned char)str[len - 1])) {
    str[len - 1] = '\0';
    len--;
  }
}

static STORAGE* storage_parse_sysfs(const char* block_name)
{
  char buffer[BUFFER_SIZE];
  STORAGE* storage = calloc(1, sizeof(*storage));
  if (!storage)
    return NULL;

  storage->device = strdup(block_name);

  // Size in sysfs is reported in 512-byte sectors
  snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/size", block_name);
  char* size_str = file_read_stripped(buffer, "\n", false);
  if (size_str) {
    storage->size_bytes = (uint64_t)strtoull(size_str, NULL, 10) * 512ULL;
    free(size_str);
  }

  snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/removable", block_name);
  char* removable_str = file_read_stripped(buffer, "\n", false);
  if (removable_str) {
    storage->removable = (atoi(removable_str) == 1);
    free(removable_str);
  }

  snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/device/model", block_name);
  storage->model = file_read_stripped(buffer, "\n", false);
  trim_trailing_spaces(storage->model);

  snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/device/serial", block_name);
  storage->serial = file_read_stripped(buffer, "\n", false);
  trim_trailing_spaces(storage->serial);

  // Some drivers put 'address', some just don't have it
  snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/device/address", block_name);
  storage->pci_slot_name = file_read_stripped(buffer, "\n", false);

  // uuid (note: uuid is usually not exposed under /sys/block directly on modern kernels, 
  // but often wwid or by-uuid via udev. However, following the snippet's exact structure:)
  snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/uuid", block_name);
  storage->uuid = file_read_stripped(buffer, "\n", false);

  return storage;
}

/**
 * Discovers all block devices present in the system by scanning /sys/class/block.
 * Ignores 'loop', 'ram', and 'zram' devices.
 * @param count Pointer to an integer where the number of found drives will be stored.
 * @return A NULL-terminated array of STORAGE pointers, or NULL on allocation failure.
 */
STORAGE** storage_get_all(int* count)
{
  *count = 0;
  DIR* dir = opendir("/sys/class/block");
  if (!dir)
    return NULL;

  STORAGE** list = calloc(MAX_STORAGES, sizeof(STORAGE*));
  if (!list) {
    closedir(dir);
    return NULL;
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL && *count < MAX_STORAGES) {
    if (entry->d_name[0] == '.') continue;
    
    // Ignore loop, ram, and zram virtual block devices
    if (strncmp(entry->d_name, "loop", 4) == 0) continue;
    if (strncmp(entry->d_name, "ram", 3) == 0) continue;
    if (strncmp(entry->d_name, "zram", 4) == 0) continue;

    STORAGE* s = storage_parse_sysfs(entry->d_name);
    if (s) {
      list[(*count)++] = s;
    }
  }

  closedir(dir);
  return list;
}

/**
 * Deep-frees a single STORAGE structure and all its strings.
 * @param storage Pointer to the STORAGE structure to free.
 */
void free_storage(STORAGE* storage)
{
  if (!storage)
    return;
  free(storage->device);
  free(storage->model);
  free(storage->serial);
  free(storage->uuid);
  free(storage->pci_slot_name);
  free(storage);
}

/**
 * Frees an array of STORAGE structures.
 * @param storages Pointer to the array of STORAGE pointers.
 * @param count Number of elements in the array.
 */
void free_storages(STORAGE** storages, int count)
{
  if (!storages)
    return;
  for (int i = 0; i < count; i++) {
    free_storage(storages[i]);
  }
  free(storages);
}

/**
 * Converts a STORAGE structure to a cJSON object.
 * @param storage Pointer to the STORAGE structure.
 * @return A cJSON object containing the STORAGE information.
 */
cJSON* storage_to_json_obj(const STORAGE* storage)
{
  cJSON* obj = cJSON_CreateObject();
  if (!storage)
    return obj;

  cJSON_AddStringToObject(obj, "device", STR_OR_UNK(storage->device));
  cJSON_AddNumberToObject(obj, "size_bytes", storage->size_bytes);
  cJSON_AddBoolToObject(obj, "removable", storage->removable);
  cJSON_AddStringToObject(obj, "model", STR_OR_UNK(storage->model));
  cJSON_AddStringToObject(obj, "serial", STR_OR_UNK(storage->serial));
  cJSON_AddStringToObject(obj, "pci_slot_name", STR_OR_UNK(storage->pci_slot_name));
  cJSON_AddStringToObject(obj, "uuid", STR_OR_UNK(storage->uuid));

  return obj;
}

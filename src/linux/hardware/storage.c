#include "base.h"
#include "file.h"
#include "io.h"
#include "storage.h"

#define MAX_STORAGES 64

static void
trim_trailing_spaces(char *str)
{
    if (str == NULL) {
        return;
    }

    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[len - 1] = '\0';
        --len;
    }
}

static STORAGE *
storage_parse_sysfs(const char *block_name)
{
    STORAGE *storage = calloc(1, sizeof(*storage));
    if (storage == NULL) {
        return NULL;
    }

    storage->device = strdup(block_name);

    /* Size in sysfs is reported in 512-byte sectors */
    char *size_str = sysfs_read_attr_fmt("\n", "/sys/class/block/%s/size", block_name);
    if (size_str != NULL) {
        storage->size_bytes = (uint64_t)strtoull(size_str, NULL, 10) * 512ULL;
        free(size_str);
    }

    char *removable_str = sysfs_read_attr_fmt("\n", "/sys/class/block/%s/removable", block_name);
    if (removable_str != NULL) {
        storage->removable = (atoi(removable_str) == 1);
        free(removable_str);
    }

    storage->model = sysfs_read_attr_fmt("\n", "/sys/class/block/%s/device/model", block_name);
    trim_trailing_spaces(storage->model);

    storage->serial = sysfs_read_attr_fmt("\n", "/sys/class/block/%s/device/serial", block_name);
    trim_trailing_spaces(storage->serial);

    /* Some drivers put 'address', some just don't have it */
    storage->pci_slot_name = sysfs_read_attr_fmt("\n", "/sys/class/block/%s/device/address", block_name);

    storage->uuid = sysfs_read_attr_fmt("\n", "/sys/class/block/%s/uuid", block_name);

    return storage;
}

static void *
storage_parse_entry(const char *name)
{
    /* Filter virtual devices */
    if (strncmp(name, "loop", 4) == 0 ||
        strncmp(name, "ram", 3) == 0 ||
        strncmp(name, "zram", 4) == 0) {
        return NULL;
    }
    
    return (void *)storage_parse_sysfs(name);
}

STORAGE **
storage_get_all(int *count)
{
    return (STORAGE **)sysfs_enumerate("/sys/class/block", storage_parse_entry, MAX_STORAGES, count);
}

void
free_storage(STORAGE *storage)
{
    if (storage == NULL) {
        return;
    }
    free(storage->device);
    free(storage->model);
    free(storage->serial);
    free(storage->uuid);
    free(storage->pci_slot_name);
    free(storage);
}

DEFINE_FREE_ARRAY(free_storages, STORAGE, free_storage)

cJSON *
storage_to_json_obj(const STORAGE *storage)
{
    cJSON *obj = cJSON_CreateObject();
    if (storage == NULL) {
        return obj;
    }

    cJSON_AddStringToObject(obj, "device", STR_OR_UNK(storage->device));
    cJSON_AddNumberToObject(obj, "size_bytes", storage->size_bytes);
    cJSON_AddBoolToObject(obj, "removable", storage->removable);
    cJSON_AddStringToObject(obj, "model", STR_OR_UNK(storage->model));
    cJSON_AddStringToObject(obj, "serial", STR_OR_UNK(storage->serial));
    cJSON_AddStringToObject(obj, "pci_slot_name", STR_OR_UNK(storage->pci_slot_name));
    cJSON_AddStringToObject(obj, "uuid", STR_OR_UNK(storage->uuid));

    return obj;
}

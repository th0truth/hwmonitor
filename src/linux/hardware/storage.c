#include "base.h"
#include "file.h"
#include "io.h"
#include "storage.h"
#include <dirent.h>
#include <limits.h>

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
    char buffer[PATH_MAX];
    STORAGE *storage = calloc(1, sizeof(*storage));
    if (storage == NULL) {
        return NULL;
    }

    storage->device = strdup(block_name);

    /* Size in sysfs is reported in 512-byte sectors */
    snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/size", block_name);
    char *size_str = file_read_stripped(buffer, "\n", false);
    if (size_str != NULL) {
        storage->size_bytes = (uint64_t)strtoull(size_str, NULL, 10) * 512ULL;
        free(size_str);
    }

    snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/removable", block_name);
    char *removable_str = file_read_stripped(buffer, "\n", false);
    if (removable_str != NULL) {
        storage->removable = (atoi(removable_str) == 1);
        free(removable_str);
    }

    snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/device/model", block_name);
    storage->model = file_read_stripped(buffer, "\n", false);
    trim_trailing_spaces(storage->model);

    snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/device/serial", block_name);
    storage->serial = file_read_stripped(buffer, "\n", false);
    trim_trailing_spaces(storage->serial);

    /* Some drivers put 'address', some just don't have it */
    snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/device/address", block_name);
    storage->pci_slot_name = file_read_stripped(buffer, "\n", false);

    /* uuid (note: uuid is usually not exposed under /sys/block directly on modern kernels, 
     * but often wwid or by-uuid via udev. However, following the snippet's exact structure:)
     */
    snprintf(buffer, sizeof(buffer), "/sys/class/block/%s/uuid", block_name);
    storage->uuid = file_read_stripped(buffer, "\n", false);

    return storage;
}

STORAGE **
storage_get_all(int *count)
{
    *count = 0;
    DIR *dir = opendir("/sys/class/block");
    if (dir == NULL) {
        return NULL;
    }

    STORAGE **list = calloc(MAX_STORAGES, sizeof(STORAGE *));
    if (list == NULL) {
        closedir(dir);
        return NULL;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && *count < MAX_STORAGES) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        /* Ignore loop, ram, and zram virtual block devices */
        if (strncmp(entry->d_name, "loop", 4) == 0) {
            continue;
        }
        if (strncmp(entry->d_name, "ram", 3) == 0) {
            continue;
        }
        if (strncmp(entry->d_name, "zram", 4) == 0) {
            continue;
        }

        STORAGE *s = storage_parse_sysfs(entry->d_name);
        if (s != NULL) {
            list[(*count)++] = s;
        }
    }

    closedir(dir);
    return list;
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

void
free_storages(STORAGE **storages, int count)
{
    if (storages == NULL) {
        return;
    }
    for (int i = 0; i < count; ++i) {
        free_storage(storages[i]);
    }
    free(storages);
}

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

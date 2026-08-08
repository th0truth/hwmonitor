#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdbool.h>
#include <cJSON.h>

typedef struct {
    char *device;
    bool removable;
    char *uuid;
    char *model;
    char *serial;
    char *pci_slot_name;
    uint64_t size_bytes;
} STORAGE;

STORAGE **storage_get_all(int *count);
void free_storage(STORAGE *storage);
void free_storages(STORAGE **storages, int count);
cJSON *storage_to_json_obj(const STORAGE *storage);

#ifdef __cplusplus
}
#endif /* __cplusplus */

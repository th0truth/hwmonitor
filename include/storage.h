/**
 * \file storage.h
 * \brief Header file for system storage and block device discovery.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdbool.h>
#include <cJSON.h>

/**
 * \struct STORAGE
 * \brief Structure to hold storage drive details.
 */
typedef struct {
    char *device;
    bool removable;
    char *uuid;
    char *model;
    char *serial;
    char *pci_slot_name;
    uint64_t size_bytes;
} STORAGE;

/**
 * \brief Discovers all block devices in the system via sysfs.
 * Returns a dynamically allocated array of STORAGE pointers.
 * \param[out] count Pointer to an integer where the number of drives will be stored.
 * \return Array of pointers to STORAGE structures.
 */
STORAGE **storage_get_all(int *count);

/**
 * \brief Frees a single STORAGE structure and its internal strings.
 * \param[in] storage Pointer to the STORAGE structure to free.
 */
void free_storage(STORAGE *storage);

/**
 * \brief Frees an array of STORAGE pointers and the array itself.
 * \param[in] storages Array of pointers.
 * \param[in] count Number of elements.
 */
void free_storages(STORAGE **storages, int count);

/**
 * \brief Converts a STORAGE structure to a cJSON object.
 * \param[in] storage Pointer to the STORAGE structure.
 * \return Pointer to a cJSON object (caller must delete).
 */
cJSON *storage_to_json_obj(const STORAGE *storage);

#ifdef __cplusplus
}
#endif /* __cplusplus */

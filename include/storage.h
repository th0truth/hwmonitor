/**
 * @file storage.h
 * @brief Header file for system storage and block device discovery.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <cJSON.h>

/**
 * @struct STORAGE
 * @brief Structure to hold storage drive details.
 */
typedef struct {
  char* device;
  uint64_t size_bytes;
  bool removable;
  char* uuid;
  char* model;
  char* serial;
  char* pci_slot_name;
} STORAGE;

/**
 * @brief Discovers all block devices in the system via sysfs.
 * Returns a dynamically allocated array of STORAGE pointers.
 * @param count Pointer to an integer where the number of drives will be stored.
 * @return Array of pointers to STORAGE structures.
 */
STORAGE** storage_get_all(int* count);

/**
 * @brief Frees a single STORAGE structure and its internal strings.
 * @param storage Pointer to the STORAGE structure to free.
 */
void free_storage(STORAGE* storage);

/**
 * @brief Frees an array of STORAGE pointers and the array itself.
 * @param storages Array of pointers.
 * @param count Number of elements.
 */
void free_storages(STORAGE** storages, int count);

/**
 * @brief Converts a STORAGE structure to a cJSON object.
 * @param storage Pointer to the STORAGE structure.
 * @return Pointer to a cJSON object (caller must delete).
 */
cJSON* storage_to_json_obj(const STORAGE* storage);

#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include "platform.h"

#if defined(HWMONITOR_UNIX)
#define BUFF_SIZE 128
#define MAX_STORAGE_DEVICES 20

typedef struct {
  char *device;
  char *model;
  char *serial;
  char *uuid;
  char* PCI_SLOT_NAME;
  bool removable;
  uint64_t size;
} STORAGE;

int getStorageInfo(STORAGE **storages);
void free_storage(STORAGE *storage);
#endif
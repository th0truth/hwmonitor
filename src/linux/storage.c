#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "storage.h"

int getStorageInfo(STORAGE **storages)
{
  FILE *fp = popen("ls /sys/block", "r");
  if (!fp) {
    return 0;
  }

  int count = 0;
  char buff[BUFF_SIZE];
  while (fgets(buff, BUFF_SIZE, fp) != NULL) {
    STORAGE *storage = malloc(sizeof *storage);
    if (!storage) {
      fprintf(stderr, "Memory allocation failed.");
      break;
    }
    memset(storage, 0, sizeof *storage);
    char path[BUFF_SIZE];
    
    buff[strcspn(buff, "\n")] = 0;
    storage->device = buff;
    
    snprintf(path, BUFF_SIZE, "/sys/block/%s/size", buff);
    storage->size = atoi(read_file(path, NULL, 0));
    
    snprintf(path, BUFF_SIZE, "/sys/block/%s/removable", buff);
    storage->removable = read_file(path, NULL, 0);
    
    snprintf(path, BUFF_SIZE, "/sys/block/%s/uuid", buff);
    storage->uuid = read_file(path, "\n", 0);
    
    snprintf(path, BUFF_SIZE, "/sys/block/%s/device/model", buff);
    storage->model = read_file(path, "\n", 0);

    snprintf(path, BUFF_SIZE, "/sys/block/%s/device/serial", buff);
    storage->serial = read_file(path, "\n", 0);

    snprintf(path, BUFF_SIZE, "/sys/block/%s/device/address", buff);
    storage->PCI_SLOT_NAME = read_file(path, "\n", 0);

    storages[count++] = storage;
  }
  return count;
}

void free_storage(STORAGE *storage)
{
  if (!storage) return;
  free(storage->model);
  free(storage->serial);
  free(storage->uuid);
  free(storage->PCI_SLOT_NAME);
  free(storage);
}
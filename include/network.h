#pragma once

#include "platform.h"

#if defined(HWMONITOR_UNIX)
#define BUFF_SIZE 128

typedef struct
{
  char *driver;
  char *pci_id;
  char *pci_subsys_id;
  char *pci_slot_name;
} NET;

NET *getNetworkInfo(const char *interface);
void free_net(NET *net);
#endif
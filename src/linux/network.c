#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "network.h"

NET *getNetworkInfo(const char *interface)
{
  NET *net = malloc(sizeof *net);
  if (net == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }

  char buff[BUFF_SIZE];
  if (strcmp(interface, "wlo1") == 0) {
    snprintf(buff, BUFF_SIZE, "/sys/class/net/wlo1/device/uevent");
    char *uevent = read_file(buff, "=", 0);
    if (uevent == NULL) {
      free(net);
      return NULL;
    }

    net->driver = findstr(uevent, "DRIVER", "\n");
      
    return net;
  }
  
  return net;
}

void free_net(NET *net)
{
  if (!net) return;
  free(net->driver);
  free(net->pci_id);
  free(net->pci_slot_name);
  free(net->pci_subsys_id);
  free(net);
}
#include "base.h"
#include "file.h"
#include "io.h"
#include "network.h"
#include <dirent.h>

Network *
network_get_info(const char *interface)
{
  if (interface == NULL) {
    return NULL;
  }

  Network *net = calloc(1, sizeof(Network));
  if (net == NULL) {
    return NULL;
  }

  net->interface = strdup(interface);

  char path[BUFFER_SIZE];
  snprintf(path, sizeof(path), "/sys/class/net/%s/device/uevent", interface);

  char *uevent = file_read_stripped(path, "=", false);
  if (uevent == NULL) {
    /* Some interfaces might not have a 'device' link (e.g., loopback, virtual) */
    return net;
  }

  net->driver         = str_find_value(uevent, "DRIVER", "\n");
  net->pci_id         = str_find_value(uevent, "PCI_ID", "\n");
  net->pci_slot_name  = str_find_value(uevent, "PCI_SLOT_NAME", "\n");
  net->pci_subsys_id  = str_find_value(uevent, "PCI_SUBSYS_ID", "\n");

  free(uevent);
  return net;
}

Network **
network_get_all(int *count)
{
  *count = 0;
  DIR *dir = opendir("/sys/class/net");
  if (dir == NULL) {
    return NULL;
  }

  Network **list = calloc(MAX_NETWORKS, sizeof(Network *));
  if (list == NULL) {
    closedir(dir);
    return NULL;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL && *count < MAX_NETWORKS) {
    if (entry->d_name[0] == '.') {
      continue;
    }

    Network *net = network_get_info(entry->d_name);
    if (net != NULL) {
      list[(*count)++] = net;
    }
  }

  closedir(dir);
  return list;
}

void
free_network(Network *net)
{
  if (net == NULL) {
    return;
  }
  free(net->interface);
  free(net->driver);
  free(net->pci_id);
  free(net->pci_slot_name);
  free(net->pci_subsys_id);
  free(net);
}

void
free_networks(Network **networks, int count)
{
  if (networks == NULL) {
    return;
  }
  for (int i = 0; i < count; ++i) {
    free_network(networks[i]);
  }
  free(networks);
}

cJSON *
network_to_json_obj(const Network *net)
{
  cJSON *obj = cJSON_CreateObject();
  if (net == NULL) {
    return obj;
  }

  cJSON_AddStringToObject(obj, "interface", STR_OR_UNK(net->interface));
  cJSON_AddStringToObject(obj, "driver", STR_OR_UNK(net->driver));
  cJSON_AddStringToObject(obj, "pci_id", STR_OR_UNK(net->pci_id));
  cJSON_AddStringToObject(obj, "pci_slot_name", STR_OR_UNK(net->pci_slot_name));
  cJSON_AddStringToObject(obj, "pci_subsys_id", STR_OR_UNK(net->pci_subsys_id));

  return obj;
}

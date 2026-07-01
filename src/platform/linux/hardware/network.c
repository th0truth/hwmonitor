/**
 * @file network.c
 * @brief Hardware discovery and parsing logic for network interfaces.
 */

#include "base.h"
#include "file.h"
#include "io.h"
#include "network.h"
#include <dirent.h>

/**
 * Populates a Network structure with data from sysfs.
 * @param interface The network interface name (e.g., "wlo1", "eth0").
 * @return Pointer to a newly allocated Network struct, or NULL on failure.
 */
Network* network_get_info(const char* interface)
{
  if (!interface)
    return NULL;

  Network* net = calloc(1, sizeof(Network));
  if (!net)
    return NULL;

  net->interface = strdup(interface);

  char path[BUFFER_SIZE];
  snprintf(path, sizeof(path), "/sys/class/net/%s/device/uevent", interface);

  char* uevent = file_read_stripped(path, "=", false);
  if (!uevent) {
    // Some interfaces might not have a 'device' link (e.g., loopback, virtual)
    return net;
  }

  net->driver         = str_find_value(uevent, "DRIVER", "\n");
  net->pci_id         = str_find_value(uevent, "PCI_ID", "\n");
  net->pci_slot_name  = str_find_value(uevent, "PCI_SLOT_NAME", "\n");
  net->pci_subsys_id  = str_find_value(uevent, "PCI_SUBSYS_ID", "\n");

  free(uevent);

  return net;
}

/**
 * Discovers all network interfaces present in the system by scanning /sys/class/net.
 * @param count Pointer to an integer where the number of found interfaces will be stored.
 * @return A NULL-terminated array of Network pointers, or NULL on allocation failure.
 */
Network** network_get_all(int* count)
{
  *count = 0;
  DIR* dir = opendir("/sys/class/net");
  if (!dir)
    return NULL;

  Network** list = calloc(MAX_NETWORKS, sizeof(Network*));
  if (!list) {
    closedir(dir);
    return NULL;
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL && *count < MAX_NETWORKS) {
    if (entry->d_name[0] == '.')
      continue;

    Network* net = network_get_info(entry->d_name);
    if (net)
      list[(*count)++] = net;
  }

  closedir(dir);
  return list;
}

/**
 * Deep-frees a Network structure.
 * @param net Pointer to the Network structure to free.
 */
void free_network(Network* net)
{
  if (!net)
    return;
  free(net->interface);
  free(net->driver);
  free(net->pci_id);
  free(net->pci_slot_name);
  free(net->pci_subsys_id);
  free(net);
}

/**
 * Frees an array of Network structures.
 * @param networks Pointer to the array of Network pointers.
 * @param count Number of elements in the array.
 */
void free_networks(Network** networks, int count)
{
  if (!networks)
    return;
  for (int i = 0; i < count; i++) {
    free_network(networks[i]);
  }
  free(networks);
}

/**
 * Converts a Network structure to a cJSON object.
 * @param net Pointer to the Network structure.
 * @return Pointer to a cJSON object (caller must delete).
 */
cJSON* network_to_json_obj(const Network* net)
{
  cJSON* obj = cJSON_CreateObject();
  if (!net)
    return obj;

  cJSON_AddStringToObject(obj, "interface", STR_OR_UNK(net->interface));
  cJSON_AddStringToObject(obj, "driver", STR_OR_UNK(net->driver));
  cJSON_AddStringToObject(obj, "pci_id", STR_OR_UNK(net->pci_id));
  cJSON_AddStringToObject(obj, "pci_slot_name", STR_OR_UNK(net->pci_slot_name));
  cJSON_AddStringToObject(obj, "pci_subsys_id", STR_OR_UNK(net->pci_subsys_id));

  return obj;
}

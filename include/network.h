/**
 * @file network.h
 * @brief Header file for network module.
 */

#pragma once

#include <stdint.h>
#include <cJSON.h>

#define MAX_NETWORKS 32

/**
 * @struct Network
 * @brief Structure to hold network interface information.
 */
typedef struct {
  char* interface;
  char* driver;
  char* pci_id;
  char* pci_slot_name;
  char* pci_subsys_id;
} Network;

cJSON* network_to_json_obj(const Network* net);
Network* network_get_info(const char* interface);
Network** network_get_all(int* count);
void free_network(Network* net);
void free_networks(Network** networks, int count);

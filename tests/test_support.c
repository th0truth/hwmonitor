#include <stdbool.h>

#include "battery.h"
#include "cpu.h"
#include "display.h"
#include "gpu.h"
#include "mainboard.h"
#include "network.h"
#include "os.h"
#include "ram.h"
#include "storage.h"
#include "file.h"

BATTERY *
battery_get_info(void)
{
    return NULL;
}

void
free_battery(BATTERY *battery)
{
    (void)battery;
}

cJSON *
battery_to_json_obj(const BATTERY *battery)
{
    (void)battery;
    return cJSON_CreateObject();
}

CPU *
cpu_get_info(void)
{
    return NULL;
}

void
free_cpu(CPU *cpu)
{
    (void)cpu;
}

cJSON *
cpu_to_json_obj(const CPU *cpu)
{
    (void)cpu;
    return cJSON_CreateObject();
}

GPU **
gpu_get_all(int *count)
{
    if (count != NULL) {
        *count = 0;
    }
    return NULL;
}

void
free_gpu(GPU *gpu)
{
    (void)gpu;
}

void
free_gpus(GPU **gpus, int count)
{
    (void)gpus;
    (void)count;
}

cJSON *
gpu_to_json_obj(const GPU *gpu)
{
    (void)gpu;
    return cJSON_CreateObject();
}

MAINBOARD *
mainboard_get_info(void)
{
    return NULL;
}

void
free_mainboard(MAINBOARD *mainboard)
{
    (void)mainboard;
}

cJSON *
mainboard_to_json_obj(const MAINBOARD *mainboard)
{
    (void)mainboard;
    return cJSON_CreateObject();
}

Network *
network_get_info(const char *interface)
{
    (void)interface;
    return NULL;
}

Network **
network_get_all(int *count)
{
    if (count != NULL) {
        *count = 0;
    }
    return NULL;
}

void
free_network(Network *net)
{
    (void)net;
}

void
free_networks(Network **networks, int count)
{
    (void)networks;
    (void)count;
}

cJSON *
network_to_json_obj(const Network *net)
{
    (void)net;
    return cJSON_CreateObject();
}

OS *
os_get_info(void)
{
    return NULL;
}

void
free_os(OS *os)
{
    (void)os;
}

cJSON *
os_to_json_obj(const OS *os)
{
    (void)os;
    return cJSON_CreateObject();
}

RAM *
ram_get_info(void)
{
    return NULL;
}

void
free_ram(RAM *ram)
{
    (void)ram;
}

cJSON *
ram_to_json_obj(const RAM *ram)
{
    (void)ram;
    return cJSON_CreateObject();
}

STORAGE **
storage_get_all(int *count)
{
    if (count != NULL) {
        *count = 0;
    }
    return NULL;
}

void
free_storage(STORAGE *storage)
{
    (void)storage;
}

void
free_storages(STORAGE **storages, int count)
{
    (void)storages;
    (void)count;
}

cJSON *
storage_to_json_obj(const STORAGE *storage)
{
    (void)storage;
    return cJSON_CreateObject();
}

void
display_battery(const BATTERY *battery)
{
    (void)battery;
}

void
display_cpu(const CPU *cpu)
{
    (void)cpu;
}

void
display_gpus(GPU **gpus, int count)
{
    (void)gpus;
    (void)count;
}

void
display_mainboard(const MAINBOARD *mainboard)
{
    (void)mainboard;
}

void
display_networks(Network **networks, int count)
{
    (void)networks;
    (void)count;
}

void
display_os(const OS *os)
{
    (void)os;
}

void
display_ram(const RAM *ram)
{
    (void)ram;
}

void
display_storages(STORAGE **storages, int count)
{
    (void)storages;
    (void)count;
}

void
display_clear(void)
{
}

bool
file_write_string(const char *filename, const char *data)
{
    (void)filename;
    (void)data;
    return true;
}

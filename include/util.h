#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "base.h"
#include "os.h"
#include "cpu.h"
#include "ram.h"
#include "gpu.h"
#include "battery.h"
#include "mainboard.h"
#include "storage.h"
#include "network.h"

typedef struct {
    char *ai_prompt;
    char *output_file;
    bool show_battery;
    bool show_cpu;
    bool show_gpu;
    bool show_mainboard;
    bool show_network;
    bool show_os;
    bool show_ram;
    bool show_storage;
    bool use_ai;
    bool use_json;
    bool watch_mode;
} Config;

typedef struct {
    BATTERY *battery;
    CPU *cpu;
    GPU **gpus;
    int gpu_count;
    MAINBOARD *mainboard;
    Network **networks;
    int network_count;
    OS *os;
    RAM *ram;
    STORAGE **storages;
    int storage_count;
} SystemHardware;

void fetch_hardware(const Config *config, SystemHardware *hw);
void free_hardware(SystemHardware *hw);

void format_size(char *target_format, uint64_t bytes, char *buffer, size_t buf_size);

cJSON *hardware_to_json(const SystemHardware *hw);
void output_json(const Config *config, const SystemHardware *hw);
void output_plaintext(const SystemHardware *hw);

void parse_arguments(int argc, char **argv, Config *config);
void print_usage(const char *prog_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "os.h"
#include "cpu.h"
#include "ram.h"
#include "gpu.h"
#include "battery.h"
#include "mainboard.h"
#include "storage.h"
#include "network.h"

void display_os(const OS *os);
void display_cpu(const CPU *cpu);
void display_ram(const RAM *ram);
void display_gpus(GPU **gpus, int count);
void display_battery(const BATTERY *battery);
void display_mainboard(const MAINBOARD *mainboard);
void display_storages(STORAGE **storages, int count);
void display_networks(Network **networks, int count);
void display_clear(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#pragma once

#include "cpu.h"
#include "ram.h"
#include "gpu.h"
#include "battery.h"

void display_cpu(const CPU* cpu);
void display_ram(const RAM* ram);
void display_gpus(GPU** gpus, int count);
void display_battery(const BATTERY* battery);
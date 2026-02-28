#pragma once

#include "cpu.h"
#include "ram.h"
#include "gpu.h"

void display_cpu(const CPU* cpu);
void display_ram(const RAM* ram);
void display_gpus(GPU** gpus, int count);
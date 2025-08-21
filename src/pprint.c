#include <stdio.h>
#include <stdlib.h>

#include "platform.h"
#include "utils.h"

#include "cpu.h" 
#include "gpu.h"
#include "mem.h"
#include "os.h"
#include "mobo.h"
#include "mainboard.h"
#include "battery.h"

#define pprint_equals(str) printf("\n============ %s ============\n\n", str)

#if defined(HWMONITOR_UNIX)
void print_CPU()
{
  CPU *cpu = getCPUinfo();
  if (cpu == NULL) {
    fprintf(stderr, "Failed to get CPU specification.\n");
  }

  pprint_equals("Central Processing Unit (CPU)");

  printf("Architecture: %s\n", cpu->arch);
  printf("CPU Vendor ID: %s", cpu->vendor_id);
  printf("CPU Family: %d\n", cpu->cpu_family);
  printf("Model: %d\n", cpu->model);
  printf("Model name: %s", cpu->model_name);
  printf("Stepping: %d\n", cpu->stepping);
  printf("Total threads: %d\n", cpu->total_threads);
  printf("Total cores: %d\n", cpu->total_cores);
  printf("Total processors: %d\n", cpu->processors);
  printf("Max CPU: %.3f MHz\n", cpu->max_MHz);
  printf("Min CPU: %.3f MHz\n", cpu->min_MHz);
  printf("Temp: %.2f °C\n", cpu->curr_temp);
  printf("Online: %s\n", cpu->online);
  free_cpu(cpu);
}

void print_GPU()
{
  GPU *gpu = getGPUinfo();
  if (gpu == NULL) {
    fprintf(stderr, "Failed to get GPU info.\n");
  }

  pprint_equals("Graphics Processing Unit (GPU)");

  printf("Vendor ID: %s\n", gpu->vendor);
  printf("Device ID: %s\n", gpu->device_id);
  printf("Driver: %s\n", gpu->driver);
  printf("Model: %s\n", gpu->model);
  printf("Subsys Vendor: %s\n", gpu->subsys_vendor);
  printf("Subsys Device: %s\n", gpu->subsys_device);
  printf("PCI ID: %s\n", gpu->pci_id);
  printf("PCI SUBSYS: %s\n", gpu->pci_subsys);
  printf("PCI SLOT NAME: %s\n", gpu->pci_slot_name);

  free_gpu(gpu);
}

void print_MEM()
{
  MEM *mem = getMEMinfo();
  if (mem == NULL) {
    fprintf(stderr, "Failed to get Memory info.");
  }
  
  pprint_equals("Memory");

  printf("Mem Total: %.2f GiB\n", calcsz("GiB", mem->total));
  printf("Mem Free: %.2f GiB\n", calcsz("GiB", mem->free));
  printf("Mem Available: %.2f GiB\n", calcsz("GiB", mem->available));
  printf("Mem Buffers: %.2f GiB\n", calcsz("GiB", mem->buffers));
  printf("Mem Cached: %.2f GiB\n", calcsz("GiB", mem->cached));
  printf("Mem SwapTotal: %.2f GiB\n", calcsz("GiB", mem->swap_total));
  printf("Mem SwapFree: %.2f GiB\n", calcsz("GiB", mem->swap_free));
  printf("Mem Zswap: %.2f GiB\n", calcsz("GiB", mem->Zswap));
  printf("Mem Zswapped: %.2f GiB\n", calcsz("GiB", mem->Zswapped));
  printf("Mem Dirty: %.2f GiB\n", calcsz("GiB", mem->dirty));
  printf("Mem perCPU: %.2f GiB\n", calcsz("GiB", mem->perCpu));

  free(mem);
}

void print_OS()
{
  OS *os = getOSinfo();
  if (os == NULL) {
    fprintf(stderr, "Failed to get operation system info.\n");
  }

  pprint_equals("Operation System (OS)");

  printf("OS: %s %d (%s)\n", os->NAME, os->VERSION_ID, os->DE);
  printf("Distro: %s\n", os->ID);
  printf("Version: %d\n", os->VERSION_ID);
  printf("DE: %s\n", os->DE);
  printf("DE ID: %s\n", os->DE_ID);
  printf("RELEASE TYPE: %s\n", os->RELEASE_TYPE);

  free_os(os);
}

void print_MOBO()
{
  MOBO *motherboard = getMOBOinfo();
  if (motherboard == NULL) {
    fprintf(stderr, "Failed to get motherboard info.\n");
  }

  pprint_equals("Motherboard");

  printf("Motherboard manufacturer: %s\n", motherboard->manufacturer);
  printf("Motherboard model: %s\n", motherboard->model);
  printf("Motherboard version: %s\n", motherboard->version);

  free_mobo(motherboard);
}

void print_BIOS()
{
  BIOS *bios = getBIOSinfo();
  if (bios == NULL) {
    fprintf(stderr, "Failed to get BIOS info.\n");
  }

  pprint_equals("Basic Input/Output System (BIOS)");

  printf("BIOS Vendor: %s\n", bios->vendor);
  printf("BIOS Version: %s\n", bios->version);
  printf("BIOS Release date: %s\n", bios->date);

  free_bios(bios);
}

void print_MAINBOARD()
{
  MAINBOARD *mainboard = getMainboardInfo();
  if (mainboard == NULL) {
    fprintf(stderr, "Failed to get mainboard info.\n");
  }
      
  pprint_equals("Mainboard");

  printf("Hardware model: %s\n", mainboard->name);
  printf("Hardware family: %s\n", mainboard->family);
  printf("Hardware serial: %s\n", mainboard->serial);
  printf("Hardware sku: %s\n", mainboard->sku);

  free_mainboard(mainboard);
}

void print_BAT()
{
  BAT *bat = getBATTERYinfo();
  if (bat == NULL) {
    fprintf(stderr, "Failed to get battery info.\n");
  }

  pprint_equals("Battery");

  printf("Capacity: %d%%\n", bat->capacity);
  printf("Voltage min design: %f\n", bat->voltage_min_design);
  printf("Voltage now: %f\n", bat->voltage_now);
  printf("Supply energy full design: %f\n", bat->energy_full_design);
  printf("Supply energy full: %f\n", bat->energy_full);
  printf("Supply energy now: %f\n", bat->energy_now);
  printf("Supply name: %s\n", bat->supply_name);
  printf("Supply type: %s\n", bat->supply_type);
  printf("Supply Capacity lavel: %s\n", bat->capacity_level);
  printf("Supply status: %s\n", bat->status);
  printf("Supply technology: %s\n", bat->technology);
  printf("Model name: %s\n", bat->model_name);
  printf("Manufacturer: %s\n", bat->manufacturer);
  printf("Serial: %s\n", bat->serial);

  free_battery(bat);
}

#endif
#include <stdio.h>
#include <stdlib.h>

#include "platform.h"
#include "utils.h"

#include "cpu.h"
#include "gpu.h"
#include "ram.h"
#include "os.h"
#include "bios.h"
#include "motherboard.h"
#include "mainboard.h"
#include "battery.h"
#include "network.h"
#include "storage.h"

#define pprint_equals(str) printf("\n============ %s ============\n\n", str)
#define pprint_error(str) fprintf(stderr, "[x] %s\n", str)
#define pprint_unknown(str) str == NULL ? "<unknown>" : str

struct helpInfo {
  char *name;
  char *description;
};

#if defined(HWMONITOR_UNIX)
void print_Help(char *argv)
{
  struct helpInfo info[] = {
    {"-h, --help", "Show this help message."},
    {"-c, --cpu", "Output CPU details."},
    {"-g, --gpu", "Output GPU details."},
    {"-r, --ram", "Output memory usage."},
    {"-b, --bat", "Output battery details."},
    {"-B, --bios", "Output BIOS information."},
    {"-s, --os", "Output Operation System information."},
    {"-m, --motherboard", "Output motherboard information."},
    {"-M, --mainboard", "Output hardware information."}
  };

  printf("usage: %s [-c] [-g] [-r] [-b] [-B] [-s] [-m] [-M]\n", argv);
  printf("\noptions:\n");

  for (int i = 0; info[i].name != NULL; i++)
    {
      printf("  %s: \t%s\n", info[i].name, info[i].description);
    }
}

void print_CPU()
{
  CPU *cpu = getCPUinfo();
  if (cpu == NULL)
  {
    fprintf(stderr, "Failed to get CPU specification.\n");
    exit(EXIT_FAILURE);
  }

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
  GPU *gpus[MAX_GPUS] = {0};
  int count = getGPUinfo(gpus);

  printf("Found %d GPU(s):\n\n", count);
 
  for (int i = 0; i < count; i++) {
    printf("GPU %d:\n", i);
    printf("  IRQ: %s\n", pprint_unknown(gpus[i]->IRQ));
    printf("  UUID: %s\n", pprint_unknown(gpus[i]->UUID));
    printf("  Video BIOS: %s\n", pprint_unknown(gpus[i]->vbios));
    printf("  Bus Type: %s\n", pprint_unknown(gpus[i]->bus_type));
    printf("  Firmware: %s\n", pprint_unknown(gpus[i]->firmware));
    printf("  Kernel Verison: %s\n", pprint_unknown(gpus[i]->kernel_version));
    printf("  Vendor ID: %s\n", gpus[i]->vendor);
    printf("  Device ID: %s\n", gpus[i]->device_id);
    printf("  Driver: %s\n", gpus[i]->driver);
    printf("  Model: %s\n", gpus[i]->model);
    printf("  Subsys Vendor: %s\n", gpus[i]->subsys_vendor);
    printf("  Subsys Device: %s\n", gpus[i]->subsys_device);
    printf("  PCI ID: %s\n", gpus[i]->pci_id);
    printf("  PCI SUBSYS: %s\n", gpus[i]->pci_subsys);
    printf("  PCI SLOT NAME: %s\n", gpus[i]->pci_slot_name);
    printf("\n");

    free_gpu(gpus[i]);
  }
}

void print_RAM()
{
  MEM *mem = getRAMinfo();
  if (mem == NULL)
  {
    fprintf(stderr, "Failed to get Memory info.");
    exit(EXIT_FAILURE);
  }

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
  if (os == NULL)
  {
    fprintf(stderr, "Failed to get operation system info.\n");
    exit(EXIT_FAILURE);
  }

  printf("OS: %s\n", os->NAME);
  printf("Distro: %s\n", pprint_unknown(os->DE_ID));
  printf("Version: %s\n", pprint_unknown(os->VERSION_ID));
  printf("DE: %s\n", pprint_unknown(os->DE));
  printf("DE ID: %s\n", pprint_unknown(os->DE_ID));
  printf("RELEASE TYPE: %s\n", pprint_unknown(os->RELEASE_TYPE));

  free_os(os);
}

void print_Motherboard()
{
  Motherboard *mobo = getMotherboardInfo();
  if (mobo == NULL)
  {
    fprintf(stderr, "Failed to get motherboard info.\n");
    exit(EXIT_FAILURE);
  }

  printf("Motherboard manufacturer: %s\n", mobo->manufacturer);
  printf("Motherboard model: %s\n", mobo->model);
  printf("Motherboard version: %s\n", mobo->version);

  free_motherboard(mobo);
}

void print_BIOS()
{
  BIOS *bios = getBIOSinfo();
  if (bios == NULL)
  {
    fprintf(stderr, "Failed to get BIOS info.\n");
    exit(EXIT_FAILURE);
  }

  printf("BIOS Vendor: %s\n", bios->vendor);
  printf("BIOS Version: %s\n", bios->version);
  printf("BIOS Release date: %s\n", bios->date);

  free_bios(bios);
}

void print_Mainboard()
{
  MAINBOARD *mainboard = getMainboardInfo();
  if (mainboard == NULL)
  {
    fprintf(stderr, "Failed to get mainboard info.\n");
    exit(EXIT_FAILURE);
  }

  printf("Hardware model: %s\n", mainboard->name);
  printf("Hardware family: %s\n", mainboard->family);
  printf("Hardware serial: %s\n", pprint_unknown(mainboard->serial));
  printf("Hardware sku: %s\n", mainboard->sku);

  free_mainboard(mainboard);
}

void print_BAT()
{
  BAT *bat = getBATTERYinfo();
  if (bat == NULL)
  {
    pprint_error("Failed to get battery info.");
    exit(EXIT_FAILURE);
  }

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

void print_NET(const char *interface)
{
  NET *net = getNetworkInfo(interface);
  if (net == NULL) {
    pprint_error("Failed to get network info.");
    exit(EXIT_FAILURE);
  }

  printf("Network driver: %s\n", net->driver);

  free_net(net);
}

void print_STORAGE()
{
  STORAGE *storages[MAX_STORAGE_DEVICES] = {0};
  int count = getStorageInfo(storages);

  for (int i = 0; i < count; i++) {
    printf("STORAGE %d: %s\n", i, storages[i]->device);
    printf("  Model: %s\n", (storages[i]->model));
    printf("  UUID: %s\n", storages[i]->uuid);
    printf("  PCI SLOT NAME: %s\n", storages[i]->PCI_SLOT_NAME);
    printf("  Size: %.2f GiB\n", calcsz("GiB", (storages[i]->size * 512 / 1024)));
    printf("  Serial: %s\n", storages[i]->serial);
    printf("  Removable: %s\n", storages[i]->removable ? "false" : "true");
    printf("\n");
    
    free_storage(storages[i]);
  }
  
}
#endif

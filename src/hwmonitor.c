#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hwmonitor.h"

int main(int argc, char *argv[]) {
  
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "--cpu") == 0) {
      CPU *cpu = getCPUinfo();
      if (cpu == NULL) {
        fprintf(stderr, "Failed to get CPU specification.\n");
        exit(EXIT_FAILURE);
      }
     
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
      continue;
    } else if (strcmp(argv[i], "--mem") == 0) {
      MEM *mem = getMEMinfo();
      if (mem == NULL) {
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
      continue;
    } else if (strcmp(argv[i], "--gpu") == 0) {
      GPU *gpu = getGPUinfo();
      if (gpu == NULL) {
        fprintf(stderr, "Failed to get GPU info.\n");
        exit(EXIT_FAILURE);
      }

      printf("Vendor ID: %s\n", gpu->vendor);
      printf("Driver: %s\n", gpu->driver);
      printf("Model: %s\n", gpu->model);

      free_gpu(gpu);
    } else if (strcmp(argv[i], "--os") == 0) {
      OS *os = getOSinfo();
      if (os == NULL) {
        fprintf(stderr, "Failed to get operation system info.\n");
        exit(EXIT_FAILURE);
      }

      printf("OS: %s %d (%s)\n", os->NAME, os->VERSION_ID, os->DE);

      free_os(os);
      continue;
    } else if (strcmp(argv[i], "--mobo") == 0) {
      MOBO *motherboard = getMOBOinfo();
      if (motherboard == NULL) {
        fprintf(stderr, "Failed to get motherboard info.\n");
        exit(EXIT_FAILURE);
      }

      printf("Motherboard manufacturer: %s\n", motherboard->manufacturer);
      printf("Motherboard model: %s\n", motherboard->model);
      printf("Motherboard version: %s\n", motherboard->version);

      free_mobo(motherboard);

      BIOS *bios = getBIOSinfo();
      if (bios == NULL) {
        fprintf(stderr, "Failed to get BIOS info.\n");
        exit(EXIT_FAILURE);
      }


      printf("BIOS Vendor: %s\n", bios->vendor);
      printf("BIOS Version: %s\n", bios->version);
      printf("BIOS Release date: %s\n", bios->date);

      free_bios(bios);

      continue;
    } else if (strcmp(argv[i], "--mainboard") == 0) {
      MAINBOARD *mainboard = getMainboardInfo();
      if (mainboard == NULL) {
        fprintf(stderr, "Failed to get mainboard info.\n");
        exit(EXIT_FAILURE);
      }
      
      printf("Hardware model: %s\n", mainboard->name);
      printf("Hardware family: %s\n", mainboard->family);
      printf("Hardware serial: %s\n", mainboard->serial);
      printf("Hardware sku: %s\n", mainboard->sku);

      free_mainboard(mainboard);
    }
  }

  return 0;
}
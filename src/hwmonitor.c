#include <stdio.h>
#include <stdlib.h>

#include "hwmonitor.h"

int main(int argc, char *argv[]) {
  
  CPU *cpu = getCPUinfo();
  if (cpu == NULL) {
    fprintf(stderr, "Failed to get CPU specs.");
    exit(EXIT_FAILURE);
  }

  printf("CPU Vendor ID: %s", cpu->vendor_id);
  printf("CPU Family: %d\n", cpu->cpu_family);
  printf("Model: %d\n", cpu->model);
  printf("Model name: %s", cpu->model_name);
  printf("Stepping: %d\n", cpu->stepping);
  printf("Total threads: %d\n", cpu->total_threads);
  printf("Total cores: %d\n", cpu->total_cores);
  printf("Total processors %d\n", cpu->processors);
  printf("Max CPU: %.3f MHz\n", cpu->max_MHz);
  printf("Min CPU: %.3f MHz\n", cpu->min_MHz);
  printf("Temp: %.2f °C\n", cpu->curr_temp);

  free(cpu);
  
  MEM *mem = getMEMinfo();
  if (cpu == NULL) {
    fprintf(stderr, "Failed to get Memory info.");
    exit(EXIT_FAILURE);
  }
  
  printf("\nMem Total: %.2f GiB\n", calcsz("GiB", mem->total));
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
  
  return 0;
}

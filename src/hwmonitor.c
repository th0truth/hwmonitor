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
  printf("Max CPU: %d MHz\n", cpu->max_freq);
  printf("Min CPU: %d MHz\n", cpu->min_freq);

  free(cpu);
  
  MEM *mem = getMEMinfo();
  if (cpu == NULL) {
    fprintf(stderr, "Failed to get Memory info.");
    exit(EXIT_FAILURE);
  }
  
  printf("\nMem Total: %d\n", mem->total);
  printf("Mem Free: %d\n", mem->free);
  printf("Mem Available: %d\n", mem->available);
  printf("Mem Buffers: %d\n", mem->buffers);
  printf("Mem Cached: %d\n", mem->cached);
  printf("Mem SwapTotal: %d\n", mem->swap_total);
  printf("Mem SwapFree: %d\n", mem->swap_free);
  printf("Mem Zswap: %d\n", mem->Zswap);
  printf("Mem Zswapped: %d\n", mem->Zswapped);
  printf("Mem Dirty: %d\n", mem->dirty);
  printf("Mem perCPU: %d\n", mem->perCpu);

  free(mem);
  
  return 0;
}

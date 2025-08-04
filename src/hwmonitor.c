#include <stdio.h>
#include <stdlib.h>

#include "hwmonitor.h"

int main(int argc, char *argv[]) {
  
  CPU *cpu = getCPUspecs();
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

  free(cpu);
  
  MEM *mem = getMEMinfo();
  if (cpu == NULL) {
    fprintf(stderr, "Failed to get Memory info.");
    exit(EXIT_FAILURE);
  }
  
  printf("Total Mem: %d\n", mem->total);
  printf("Mem Free: %d\n", mem->free);

  free(mem);
  
  return 0;
}

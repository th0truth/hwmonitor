#include "stdio.h"
#include <stdlib.h>

#include "utils.h"
#include "cpu.h"

CPU *getCPUspecs()
{
  CPU *cpu = (CPU*)malloc(sizeof(CPU));
  if (cpu == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    free(cpu);
    exit(EXIT_FAILURE);
  }

  char *cpu_info = read_file("/proc/cpuinfo");

  cpu->total_cores = atoi(findstr(cpu_info, "cpucores:", "\n"));
  cpu->total_threads = atoi(findstr(cpu_info, "sibling:", "\n"));
  cpu->model_name = findstr(cpu_info, "modelname:", "\n");
  
  free(cpu_info);
  return cpu;
}
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

  char regex[] = {'\t'};
  char *cpu_info = read_file("/proc/cpuinfo", regex);

  cpu->total_cores = atoi(findstr(cpu_info, "cpu cores: ", "\n"));
  cpu->total_threads = atoi(findstr(cpu_info, "siblings: ", "\n"));
  cpu->model_name = findstr(cpu_info, "model name: ", "\n");

  free(cpu_info);
  return cpu;
}
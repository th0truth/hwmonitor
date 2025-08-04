#include "stdio.h"
#include <stdlib.h>

#include "utils.h"
#include "cpu.h"

CPU *getCPUspecs()
{
  CPU *cpu = (CPU*)malloc(sizeof(CPU));
  if (cpu == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    return NULL;
  }

  char regex[] = {'\t', ':'};
  char *cpuinfo = read_file("/proc/cpuinfo", regex);
  if (cpuinfo == NULL) {
    free(cpu);
    return NULL;
  }

  cpu->vendor_id = findstr(cpuinfo, "vendor_id", "\n");
  cpu->cpu_family = atoi(findstr(cpuinfo, "cpu family", "\n"));
  cpu->model = atoi(findstr(cpuinfo, "model", "\n"));
  cpu->model_name = findstr(cpuinfo, "model name", "\n");
  cpu->stepping = atoi(findstr(cpuinfo, "stepping", "\n"));
  cpu->total_threads = atoi(findstr(cpuinfo, "siblings", "\n"));
  cpu->total_cores = atoi(findstr(cpuinfo, "cpu cores", "\n"));
  cpu->flags = findstr(cpuinfo, "flags", "\n");

  free(cpuinfo);
  return cpu;
}
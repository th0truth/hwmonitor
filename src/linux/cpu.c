#include "stdio.h"
#include <stdlib.h>

 #include "utils.h"
#include "cpu.h"

CPU *getCPUspecs()
{
  CPU *cpu;

  char *cpuinfo = read_file("/proc/cpuinfo");

  cpu->model_name = findstr(cpuinfo, "cpucores:", "\n");

  free(cpuinfo);  
}
#include "stdio.h"
#include <stdlib.h>

#include "utils.h"
#include "mem.h"

MEM *getMEMinfo()
{
  MEM *mem = (MEM*)malloc(sizeof(MEM));
  if (mem == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    return NULL;
  }

  char rmch[] = {' ', ':'};
  char *meminfo = read_file("/proc/meminfo", rmch);
  if (meminfo == NULL) {
    free(mem);
    return NULL;
  }

  mem->total      = atof(findstr(meminfo, "MemTotal", "kB"));
  mem->free       = atof(findstr(meminfo, "MemFree", "kB"));
  mem->available  = atof(findstr(meminfo, "MemAvailable", "kB"));
  mem->buffers    = atof(findstr(meminfo, "Buffers", "kB"));
  mem->cached     = atof(findstr(meminfo, "Cached", "kB"));
  mem->swap_total = atof(findstr(meminfo, "SwapTotal", "kB"));
  mem->swap_free  = atof(findstr(meminfo, "SwapFree", "kB"));
  mem->Zswap      = atof(findstr(meminfo, "Zswap", "kB"));
  mem->Zswapped   = atof(findstr(meminfo, "Zswapped", "kB"));
  mem->dirty      = atof(findstr(meminfo, "Dirty", "kB"));
  mem->perCpu     = atof(findstr(meminfo, "Percpu", "kB"));

  free(meminfo);
  return mem;
}
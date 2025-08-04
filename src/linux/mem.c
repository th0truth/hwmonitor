#include "stdio.h"
#include <stdlib.h>

#include "utils.h"
#include "mem.h"

MEM *getMEMinfo()
{
  MEM *mem = (MEM*)malloc(sizeof(MEM));
  if (mem == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    free(mem);
    exit(EXIT_FAILURE);
  }

  char regex[] = {' ', ':'};
  char *meminfo = read_file("/proc/meminfo", regex);
  if (meminfo == NULL) {
    free(mem);
    return NULL;
  }

  mem->total = atoi(findstr(meminfo, "MemTotal", "kB"));
  mem->free = atoi(findstr(meminfo, "MemFree", "kB"));
  mem->available = atoi(findstr(meminfo, "MemAvailable", "kB"));
  mem->buffers = atoi(findstr(meminfo, "Buffers", "kB"));
  mem->cached = atoi(findstr(meminfo, "Cached", "kB"));
  mem->swap_total = atoi(findstr(meminfo, "SwapTotal", "kB"));
  mem->swap_free = atoi(findstr(meminfo, "SwapFree", "kB"));
  mem->Zswap = atoi(findstr(meminfo, "Zswap", "kB"));
  mem->Zswapped = atoi(findstr(meminfo, "Zswapped", "kB"));
  mem->dirty = atoi(findstr(meminfo, "Dirty", "kB"));
  mem->perCpu = atoi(findstr(meminfo, "Percpu", "kB"));

  free(meminfo);
  return mem;
}
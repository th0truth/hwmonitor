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

  mem->total = atoi(findstr(meminfo, "MemTotal", "\n"));

  free(meminfo);
  return mem;
}
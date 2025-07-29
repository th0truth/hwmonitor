#include "stdio.h"
#include <stdlib.h>
#include "utils/utils.h"

void getTotalMem() {
  char *meminfo = read_file("/proc/meminfo");

  printf("RAM: %s kB", findstr(meminfo, "MemTotal:", "kB"));
  
  free(meminfo);
}
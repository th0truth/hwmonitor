#include <stdio.h>
#include <stdlib.h>
#include "hwmonitor.h"

int main(int argc, char *argv[]) {
  
  char *buffer = read_proc("/proc/cpuinfo");

  printf("%s", buffer);

  free(buffer);

  return 0;
}

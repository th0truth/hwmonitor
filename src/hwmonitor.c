#include <stdio.h>
#include <stdlib.h>

#include "hwmonitor.h"

int main(int argc, char *argv[]) {
  
  CPU *cpu = getCPUspecs();

  printf("Total cores: %d\n", cpu->total_cores);
  printf("Total threads: %d\n", cpu->total_threads);
  printf("Model name: %s\n", cpu->model_name);

  free(cpu);
  
  return 0;
}

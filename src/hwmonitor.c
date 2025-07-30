#include <stdio.h>
#include <stdlib.h>

#include "hwmonitor.h"

int main(int argc, char *argv[]) {
  
  // getTotalMem();
  CPU *cpu = getCPUspecs();

  printf("%s", cpu->model_name);

  return 0;
}

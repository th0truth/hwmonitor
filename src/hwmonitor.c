#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hwmonitor.h"

int main(int argc, char *argv[]) {
  
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "--cpu") == 0) {
      print_CPU();
      continue;
    } else if (strcmp(argv[i], "--mem") == 0) {
      print_MEM();
      continue;
    } else if (strcmp(argv[i], "--gpu") == 0) {
      print_GPU();
      continue;
    } else if (strcmp(argv[i], "--os") == 0) {
      print_OS();
      continue;
    } else if (strcmp(argv[i], "--mobo") == 0) {
      print_MOBO();
      continue;
    } else if (strcmp(argv[i], "--bios") == 0) {
      print_BIOS();
      continue; 
    } else if (strcmp(argv[i], "--mainboard") == 0) {
      print_MAINBOARD();
      continue;
    } else if (strcmp(argv[i], "--battery") == 0) {
      print_BAT();
      continue;
    }
  }

  return 0;
}
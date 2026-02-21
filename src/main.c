#include "base.h"

#include "cpu.h"

int main(int argc, char** argv)
{
  CPU* cpu = cpu_get_info();

  printf("%s", cpu->arch);

  free(cpu);
  
  return 0;
}
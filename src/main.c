#include "base.h"

#include "cpu.h"

int main(int argc, char** argv)
{
  CPU* cpu;
  cpu = cpu_get_info();

  free(cpu);
  
  return 0;
}
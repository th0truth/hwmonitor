#include "base.h"

#include "cpu.h"

void display_cpu(const CPU* cpu)
{
  if (cpu == NULL) {
    return;
  }

  printf("%-10s %s (%s)\n", "Vendor:", STR_OR_UNK(cpu->vendor_id), STR_OR_UNK(cpu->arch));
  printf("%-10s %s %s\n", "Model:", STR_OR_UNK(cpu->model_name));
  printf("Physical Cores: %u\n", cpu->total_cores);
  printf("Logical cores: %u\n", cpu->total_threads);
  printf("Max freq: %.2f MHz\n", cpu->max_MHz);
  printf("Min freq: %.2f MHz\n", cpu->min_MHz);
  printf("Flags: %s", cpu->flags);
  printf("\n");
};

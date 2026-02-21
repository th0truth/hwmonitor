#include "base.h"
#include "file.h"

#include "io.h"
#include "cpu.h"

char* cpu_get_arch(const char* flags)
{
  char* flag = str_find_value(flags, "lm", NULL);
  if (!flag) {
    return "x86";
  } else {
    return "x86_64";
  }
}

CPU* cpu_get_info()
{
  CPU* cpu = malloc(sizeof(CPU));
  if (!cpu) {
    return NULL;
  }

  char* cpuinfo = file_read_stripped("/proc/cpuinfo", ":\t", false);
  if (!cpuinfo) {
    free(cpu);
    return NULL;
  }

  cpu->vendor_id      = str_find_value(cpuinfo, "vendor_id", "\n");
  cpu->model_name     = str_find_value(cpuinfo, "model name", "\n");
  cpu->flags          = str_find_value(cpuinfo, "flags", "\n");
  cpu->arch           = cpu_get_arch(cpu->flags);
  cpu->cpu_family     = atoi(str_find_value(cpuinfo, "cpu family", "\n"));
  cpu->model          = atoi(str_find_value(cpuinfo, "model", "\n"));
  cpu->stepping       = atoi(str_find_value(cpuinfo, "stepping", "\n"));
  cpu->total_cores    = atoi(str_find_value(cpuinfo, "cpu cores", "\n"));
  cpu->total_threads  = atoi(str_find_value(cpuinfo, "siblings", "\n"));

  free(cpuinfo);

  return cpu;
}
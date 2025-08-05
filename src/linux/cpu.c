#include "stdio.h"
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include "utils.h"
#include "cpu.h"

CPU *getCPUspecs()
{
  CPU *cpu = (CPU*)malloc(sizeof(CPU));
  if (cpu == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    return NULL;
  }

  char regex[] = {'\t', ':'};
  char *cpuinfo = read_file("/proc/cpuinfo", regex);
  if (cpuinfo == NULL) {
    free(cpu);
    return NULL;
  }

  cpu->vendor_id = findstr(cpuinfo, "vendor_id", "\n");
  cpu->cpu_family = atoi(findstr(cpuinfo, "cpu family", "\n"));
  cpu->model = atoi(findstr(cpuinfo, "model", "\n"));
  cpu->model_name = findstr(cpuinfo, "model name", "\n");
  cpu->stepping = atoi(findstr(cpuinfo, "stepping", "\n"));
  cpu->total_threads = atoi(findstr(cpuinfo, "siblings", "\n"));
  cpu->total_cores = atoi(findstr(cpuinfo, "cpu cores", "\n"));
  cpu->flags = findstr(cpuinfo, "flags", "\n");

  free(cpuinfo);
  return cpu;
}

int64_t getCPUCoreMaxFreq_MHz(unsigned core_id)
{
  char buff[100];
  snprintf(buff, sizeof(buff), "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_max_freq", core_id);

  char rmch[] = {' '};
  char *cpufreq = read_file(buff, rmch);
  if (cpufreq == NULL) {
    return -1;
  }

  int64_t kHz = atoi(cpufreq);
  if (kHz > -1) {
    return kHz / 1000;
  }

  free(cpufreq);
  return -1;
}

int64_t getCPUCoreMinFreq_MHz(unsigned core_id)
{
  char buff[100];
  snprintf(buff, sizeof(buff), "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_min_freq", core_id);

  char rmch[] = {' '};
  char *cpufreq = read_file(buff, rmch);
  if (cpufreq == NULL) {
    return -1;
  }

  int64_t kHz = atoi(cpufreq);
  if (kHz > -1) {
    return kHz / 1000;
  }

  free(cpufreq);
  return -1;
}

int64_t getCPUCoreRegularFreq_MHz(unsigned core_id)
{
  char buff[100];
  snprintf(buff, sizeof(buff), "/sys/devices/system/cpu/cpu%d/cpufreq/base_frequency", core_id);

  char rmch[] = {' '};
  char *cpufreq = read_file(buff, rmch);
  if (cpufreq == NULL) {
    return -1;
  }

  int64_t kHz = atoi(cpufreq);
  if (kHz > -1) {
    return kHz / 1000;
  }

  free(cpufreq);
  return -1;
}
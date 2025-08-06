#include "stdio.h"
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "utils.h"
#include "cpu.h"

CPU *getCPUinfo()
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

int64_t getCPUCoreMaxFreq_MHz(unsigned short core_id)
{
  char buff[BUFF_SIZE];
  snprintf(buff, BUFF_SIZE, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_max_freq", core_id);
  
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

int64_t getCPUCoreMinFreq_MHz(unsigned short core_id)
{
  char buff[BUFF_SIZE];
  snprintf(buff, BUFF_SIZE, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_min_freq", core_id);

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

int64_t getCPUCoreRegFreq_MHz(unsigned short core_id)
{
  char buff[BUFF_SIZE];
  snprintf(buff, BUFF_SIZE, "/sys/devices/system/cpu/cpu%d/cpufreq/base_frequency", core_id);

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

float getCPUCurrTemp_Celsius()
{
  char buff[BUFF_SIZE];
  int i = 0;
  char rmch[] = {' ', '\n'};
  for (;;) {
    snprintf(buff, BUFF_SIZE, "/sys/class/thermal/thermal_zone%d/type", ++i);    
    char *thtype = read_file(buff, rmch);
    if (thtype == NULL) {
      return -1;
    }  
    thtype[strcspn(thtype, "\xff")] = '\0';
    if (strcmp(thtype, "x86_pkg_temp") == 0) {
      break;
    }
    free(thtype);
  }
  
  snprintf(buff, BUFF_SIZE, "/sys/class/thermal/thermal_zone%d/temp", i);
  char *thtemp = (read_file(buff, rmch));
  if (thtemp == NULL) {
    return -1;
  }
  
  float temp = atoi(thtemp) / 1000;
  
  free(thtemp);
  return temp;
}

uint64_t getCPUMaxFreq_MHz(unsigned short t_cores)
{
  unsigned max = 0, tmp = 0;
  for (int i = 0; i < t_cores; i++)
  {
    tmp = getCPUCoreMaxFreq_MHz(i);
    if (tmp > max) {
      max = tmp;
    }
  }
  return max;
}

uint64_t getCPUMinFreq_MHz(unsigned short t_cores)
{
  unsigned min = 0, tmp = 0;
  for (int i = 0; i < t_cores; i++)
  {
    tmp = getCPUCoreMaxFreq_MHz(i);
    if (tmp < min) {
      min = tmp;
    }
  }
  return min;
}
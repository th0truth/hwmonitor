#include "stdio.h"
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "utils.h"
#include "cpu.h"

int16_t getCPUTotalProcessors()
{
  char *cpus = read_file("/sys/devices/system/cpu/online", "-");
  if (cpus == NULL) {
    return -1;
  }
  
  int32_t processors = atoi(cpus);
  
  free(cpus);
  return processors;
}

int64_t getCPUCoreMaxFreq_MHz(unsigned short core_id)
{
  char buff[BUFF_SIZE];
  snprintf(buff, BUFF_SIZE, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_max_freq", core_id);
  
  char *cpufreq = read_file(buff, NULL);
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
  char *cpufreq = read_file(buff, NULL);
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
  char *cpufreq = read_file(buff, NULL);
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

float getCPUMaxFreq_MHz(unsigned short t_processors)
{
  char buff[BUFF_SIZE];
  float max = 0, tmp = 0;
  for (int i = 0; i < t_processors; i++)
  {
    snprintf(buff, BUFF_SIZE, "/sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_max_freq", i);
    char *kHz = read_file(buff, NULL);
    if (kHz == NULL) {
      return -1;
    }
    tmp = atof(kHz);
    if (tmp > max) {
      max = tmp;
    }
    free(kHz);
  }
  // convert kHz to MHz
  max /= 1000;
  return max;
}

float getCPUMinFreq_MHz(unsigned short t_processors)
{
  char buff[BUFF_SIZE];
  float min = 0, tmp = 0;
  for (int i = 0; i < t_processors; i++)
  {
    snprintf(buff, BUFF_SIZE, "/sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_min_freq", i);
    char *kHz = read_file(buff, NULL);
    if (kHz == NULL) {
      return -1;
    }
    tmp = atof(kHz);
    if (tmp != 0 || tmp < min) {
      min = tmp;
    }
  }
  // convert kHz to MHz
  min /= 1000;
  return min;
}

float getCPUCurrTemp_Celsius()
{
  char buff[BUFF_SIZE];
  int i = 0;
  for (;;) {
    snprintf(buff, BUFF_SIZE, "/sys/class/thermal/thermal_zone%d/type", ++i);
    char *thtype = read_file(buff, "\n");
    thtype[strcspn(thtype, "\xff")] = '\0';
    if (thtype == NULL) {
      return -1;
    }  
    if (strcmp(thtype, "x86_pkg_temp") == 0) {
      break;
    }
    free(thtype);
  }
  
  snprintf(buff, BUFF_SIZE, "/sys/class/thermal/thermal_zone%d/temp", i);
  char *thtemp = (read_file(buff, NULL));
  if (thtemp == NULL) {
    return -1;
  }
  
  float temp = atoi(thtemp) / 1000;
  
  free(thtemp);
  return temp;
}

CPU *getCPUinfo()
{
  CPU *cpu = (CPU*)malloc(sizeof(CPU));
  if (cpu == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    return NULL;
  }

  char rmch[] = {'\t', ':'};
  char *cpuinfo = read_file("/proc/cpuinfo", rmch);
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
  cpu->processors = getCPUTotalProcessors();
  cpu->max_MHz = getCPUMaxFreq_MHz(cpu->processors);
  cpu->min_MHz = getCPUMinFreq_MHz(cpu->processors);
  cpu->curr_temp = getCPUCurrTemp_Celsius();
  cpu->flags = findstr(cpuinfo, "flags", "\n");

  free(cpuinfo);
  return cpu;
}
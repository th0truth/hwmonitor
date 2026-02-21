#include "base.h"
#include "file.h"
#include "io.h"
#include "cpu.h"

/**
 * Parses the number of online logical processors from sysfs.
 * Linux reports this as a range (e.g., "0-7") or a single number.
 * @return Number of online cores, or -1 on failure.
 */
int16_t cpu_get_total_cores(void)
{
  char* online = file_read_stripped("/sys/devices/system/cpu/online", "\n", false);
  if (online == NULL) {
    return -1;
  }

  int start, end;
  int16_t count = 0;
  
  if (sscanf(online, "%d-%d", &start, &end) == 2) {
    count = (end - start) + 1;
  } else if (sscanf(online, "%d", &start) == 1) {
    count = 1;
  }

  free(online);
  return count;
}

/**
 * Retrieves CPU frequency in MHz for a specific core.
 * @param core_id The logical core index.
 * @param filename The sysfs file to read (e.g., "scaling_max_freq").
 * @return Frequency in MHz, or -1 on failure.
 */
int16_t cpu_get_total_freq_mhz(uint16_t core_id, const char* filename)
{
  char buffer[BUFFER_SIZE];
  snprintf(buffer, sizeof(buffer), "/sys/devices/system/cpu/cpu%u/cpufreq/%s", core_id, filename);

  char* cpu_freq = file_read_stripped(buffer, "\n", true);
  if (cpu_freq == NULL) {
    return -1;
  }

  int32_t khz = atoi(cpu_freq);
  free(cpu_freq);

  if (khz <= 0) {
    return -1;
  }
  
  return (int16_t)(khz / 1000);
} 

/**
 * Determines CPU architecture based on flags.
 * @param flags The CPU flags string from /proc/cpuinfo.
 * @return Static string "x86" or "x86_64".
 */
char* cpu_get_arch(const char* flags)
{
  if (flags == NULL) {
    return "x86";
  }
  
  char* flag = str_find_value(flags, "lm", NULL);
  if (flag == NULL) {
    return "x86";
  } else {
    free(flag);
    return "x86_64";
  }
}

/**
 * Converts a CPU structure to a cJSON object.
 * @param cpu Pointer to the CPU structure.
 * @return Pointer to a cJSON object (caller must delete).
 */
cJSON* cpu_to_json_obj(const CPU* cpu)
{
  cJSON* obj = cJSON_CreateObject();
  if (cpu == NULL) {
    return obj;
  }

  cJSON_AddStringToObject(obj, "vendor", STR_OR_UNK(cpu->vendor_id));
  cJSON_AddStringToObject(obj, "model_name", STR_OR_UNK(cpu->model_name));
  cJSON_AddStringToObject(obj, "flags", STR_OR_UNK(cpu->flags));
  cJSON_AddStringToObject(obj, "arch", STR_OR_UNK(cpu->arch));

  cJSON_AddNumberToObject(obj, "max_freq_mhz", cpu->max_MHz);
  cJSON_AddNumberToObject(obj, "min_freq_mhz", cpu->min_MHz);
  cJSON_AddNumberToObject(obj, "online_cores", cpu->online_cores);
  cJSON_AddNumberToObject(obj, "cpu_family", cpu->cpu_family);
  cJSON_AddNumberToObject(obj, "model", cpu->model);
  cJSON_AddNumberToObject(obj, "stepping", cpu->stepping);
  cJSON_AddNumberToObject(obj, "total_cores", cpu->total_cores);
  cJSON_AddNumberToObject(obj, "total_threads", cpu->total_threads);

  return obj;
}

/**
 * Populates a CPU structure with data from /proc/cpuinfo and sysfs.
 * @return Pointer to a newly allocated CPU struct, or NULL on failure.
 */
CPU* cpu_get_info(void)
{
  CPU* cpu = calloc(1, sizeof(CPU));
  if (cpu == NULL) {
    return NULL;
  }

  char* cpu_info = file_read_stripped("/proc/cpuinfo", ":\t", false);
  if (cpu_info == NULL) {
    free(cpu);
    return NULL;
  }

  cpu->vendor_id      = str_find_value(cpu_info, "vendor_id", "\n");
  cpu->model_name     = str_find_value(cpu_info, "model name", "\n");
  cpu->flags          = str_find_value(cpu_info, "flags", "\n");
  cpu->arch           = cpu_get_arch(cpu->flags);
  
  cpu->online_cores   = cpu_get_total_cores();
  cpu->max_MHz        = cpu_get_total_freq_mhz((cpu->online_cores - 1), "scaling_max_freq");
  cpu->min_MHz        = cpu_get_total_freq_mhz((cpu->online_cores - 1), "scaling_min_freq");
  
  cpu->cpu_family     = atoi(str_find_value(cpu_info, "cpu family", "\n"));
  cpu->model          = atoi(str_find_value(cpu_info, "model", "\n"));
  cpu->stepping       = atoi(str_find_value(cpu_info, "stepping", "\n"));
  cpu->total_cores    = atoi(str_find_value(cpu_info, "cpu cores", "\n"));
  cpu->total_threads  = atoi(str_find_value(cpu_info, "siblings", "\n"));

  free(cpu_info);
  return cpu;
}

/**
 * Deep-frees a CPU structure.
 * @param cpu Pointer to the CPU structure to free.
 */
void free_cpu(CPU* cpu)
{
  if (cpu == NULL) {
    return;
  }
  free(cpu->vendor_id);
  free(cpu->model_name);
  free(cpu->flags);
  free(cpu);
}

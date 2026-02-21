#include "base.h"
#include "file.h"
#include "io.h"

#include <cJSON.h>
#include "cpu.h"

int16_t cpu_get_total_cores()
{
  char* online = file_read_stripped("/sys/devices/system/cpu/online", "\n", false);
  if (!online) {
    return -1;
  }

  int start;
  int end;
  int16_t count = 0;
  if (sscanf(online, "%d-%d", &start, &end) == 2) {
    count = (end - start) + 1;
  } else if (sscanf(online, "%d", &start) == 1) {
    count = 1;
  }

  free(online);

  return count;
}

int16_t cpu_get_total_freq_mhz(uint16_t core_id, const char* filename)
{
  char buffer[BUFFER_SIZE];
  snprintf(buffer, sizeof(buffer), "/sys/devices/system/cpu/cpu%u/cpufreq/%s", core_id, filename);

  char* cpu_freq = file_read_stripped(buffer, "\n", true);
  if (!cpu_freq) {
    return -1;
  }

  // Convert value
  int32_t khz = atoi(cpu_freq);

  free(cpu_freq);

  if (khz <= 0) {
    return -1;
  }

  return khz / 1000;
} 

char* cpu_get_arch(const char* flags)
{
  char* flag = str_find_value(flags, "lm", NULL);
  if (!flag) {
    return "x86";
  } else {
    return "x86_64";
  }
}

char* cpu_to_json(const CPU* cpu)
{
  if (!cpu) {
    return NULL;
  }

  // Create the root JSON object
  cJSON* json = cJSON_CreateObject();
  
  // Add string values
  cJSON_AddStringToObject(json, "vendor", STR_OR_UNK(cpu->vendor_id));
  cJSON_AddStringToObject(json, "model_name", STR_OR_UNK(cpu->model_name));
  cJSON_AddStringToObject(json, "flags", STR_OR_UNK(cpu->flags));
  cJSON_AddStringToObject(json, "arch", STR_OR_UNK(cpu->arch));

  // Add number values
  cJSON_AddNumberToObject(json, "max_freq_mhz", cpu->max_MHz);
  cJSON_AddNumberToObject(json, "min_freq_mhz", cpu->min_MHz);
  cJSON_AddNumberToObject(json, "online_cores", cpu->online_cores);
  cJSON_AddNumberToObject(json, "cpu_family", cpu->cpu_family);
  cJSON_AddNumberToObject(json, "model", cpu->model);
  cJSON_AddNumberToObject(json, "stepping", cpu->stepping);
  cJSON_AddNumberToObject(json, "total_cores", cpu->total_cores);
  cJSON_AddNumberToObject(json, "total_threads", cpu->total_threads);

  // Convert the cJSON object to a JSON string
  char* json_string = cJSON_Print(json);

  // Clean up the JSON object memory
  cJSON_Delete(json);

  return json_string;
}

CPU* cpu_get_info()
{
  CPU* cpu = malloc(sizeof(CPU));
  if (!cpu) {
    return NULL;
  }

  // Read from file
  char* cpu_info = file_read_stripped("/proc/cpuinfo", ":\t", false);
  if (!cpu_info) {
    free(cpu);
    return NULL;
  }

  // Add values to a CPU obj
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
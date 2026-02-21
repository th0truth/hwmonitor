#include "base.h"
#include "file.h"
#include "io.h"

#include <cJSON.h>
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
  char* cpuinfo = file_read_stripped("/proc/cpuinfo", ":\t", false);
  if (!cpuinfo) {
    free(cpu);
    return NULL;
  }

  // Add values to a CPU obj
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
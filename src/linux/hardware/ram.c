#include "base.h"
#include "file.h"
#include "io.h"
#include "ram.h"

/**
 * Discovers and parses memory information from /proc/meminfo.
 * @return Pointer to a newly allocated RAM struct, or NULL on failure.
 */
RAM* ram_get_info(void)
{
  RAM* ram = calloc(1, sizeof(*ram));
  if (ram == NULL) {
    return NULL;
  }

  char* info = file_read_stripped("/proc/meminfo", "\n", false);
  if (info == NULL) {
    free(ram);
    return NULL;
  }

  // Use the helper to populate the RAM fields in kB
  ram->total      = str_parse_value(info, "MemTotal", "kB");
  ram->free       = str_parse_value(info, "MemFree", "kB");
  ram->available  = str_parse_value(info, "MemAvailable", "kB");
  ram->buffers    = str_parse_value(info, "Buffers", "kB");
  ram->cached     = str_parse_value(info, "Cached", "kB");
  ram->swap_total = str_parse_value(info, "SwapTotal", "kB");
  ram->swap_free  = str_parse_value(info, "SwapFree", "kB");
  ram->zswap      = str_parse_value(info, "Zswap", "kB");
  ram->zswapped   = str_parse_value(info, "Zswapped", "kB");
  ram->dirty      = str_parse_value(info, "Dirty", "kB");
  ram->per_cpu    = str_parse_value(info, "Percpu", "kB");

  free(info);
  return ram;
}

/**
 * Frees a RAM structure.
 * @param ram Pointer to the RAM structure to free.
 */
void free_ram(RAM* ram)
{
  if (ram != NULL) {
    free(ram);
  }
}

/**
 * Converts a RAM structure to a cJSON object.
 * @param ram Pointer to the RAM structure.
 * @return Pointer to a cJSON object (caller must delete).
 */
cJSON* ram_to_json_obj(const RAM* ram)
{
  cJSON* obj = cJSON_CreateObject();
  if (ram == NULL) {
    return obj;
  }

  cJSON_AddNumberToObject(obj, "total", ram->total);
  cJSON_AddNumberToObject(obj, "free", ram->free);
  cJSON_AddNumberToObject(obj, "available", ram->available);
  cJSON_AddNumberToObject(obj, "buffers", ram->buffers);
  cJSON_AddNumberToObject(obj, "cached", ram->cached);
  cJSON_AddNumberToObject(obj, "swap_total", ram->swap_total);
  cJSON_AddNumberToObject(obj, "swap_free", ram->swap_free);
  cJSON_AddNumberToObject(obj, "zswap", ram->zswap);
  cJSON_AddNumberToObject(obj, "zswapped", ram->zswapped);
  cJSON_AddNumberToObject(obj, "dirty", ram->dirty);
  cJSON_AddNumberToObject(obj, "per_cpu", ram->per_cpu);

  return obj;
}

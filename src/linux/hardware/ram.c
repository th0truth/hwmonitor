#include "base.h"
#include "file.h"
#include "io.h"
#include "ram.h"

/**
 * Internal helper for parsing memory values in kB from meminfo.
 * @param info The raw string contents of /proc/meminfo.
 * @param search The key to search for (e.g., "MemTotal").
 * @return Value in kB as a 64-bit integer, or 0 on failure.
 */
static uint64_t parse_mem_value(const char* info, const char* search)
{
  char* val = str_find_value(info, search, "kB");
  if (val == NULL) {
    return 0;
  }
  
  uint64_t result = (uint64_t)atoll(val);
  free(val);
  return result;
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
  ram->total      = parse_mem_value(info, "MemTotal");
  ram->free       = parse_mem_value(info, "MemFree");
  ram->available  = parse_mem_value(info, "MemAvailable");
  ram->buffers    = parse_mem_value(info, "Buffers");
  ram->cached     = parse_mem_value(info, "Cached");
  ram->swap_total = parse_mem_value(info, "SwapTotal");
  ram->swap_free  = parse_mem_value(info, "SwapFree");
  ram->zswap      = parse_mem_value(info, "Zswap");
  ram->zswapped   = parse_mem_value(info, "Zswapped");
  ram->dirty      = parse_mem_value(info, "Dirty");
  ram->per_cpu    = parse_mem_value(info, "Percpu");

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

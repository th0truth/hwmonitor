#pragma once

#include <inttypes.h>
#include <cJSON.h>

/**
 * Structure to hold battery metrics and vendor information.
 */
typedef struct {
  uint16_t capacity;
  float voltage_min_design;
  float voltage_now;
  float energy_full_design;
  float energy_full;
  float energy_now;
  char* supply_name;
  char* supply_type;
  char* capacity_level;
  char* status;
  char* technology;
  char* model_name;
  char* manufacturer;
  char* serial;
} BATTERY;

/**
 * Discovers and parses battery information from /sys/class/power_supply.
 * @return Pointer to a newly allocated BATTERY struct, or NULL on failure.
 */
BATTERY* battery_get_info(void);

/**
 * Deep-frees a BATTERY structure and its internal strings.
 * @param battery Pointer to the structure to free.
 */
void free_battery(BATTERY* battery);

/**
 * Converts a BATTERY structure to a cJSON object.
 * @param battery Pointer to the BATTERY structure.
 * @return Pointer to a cJSON object (caller must delete).
 */
cJSON* battery_to_json_obj(const BATTERY* battery);

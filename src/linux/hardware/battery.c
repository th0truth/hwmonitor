/**
 * @file battery.c
 * @brief Hardware discovery and parsing logic for system batteries.
 */

#include "base.h"
#include "file.h"
#include "io.h"
#include "battery.h"

// Standard Linux sysfs unit conversion factor (micro to standard)
#define SYSFS_UNIT_CONVERSION 1000000.0

/**
 * Discovers and parses primary battery information from sysfs.
 * @return Pointer to a newly allocated BATTERY struct, or NULL if no battery is found.
 */
BATTERY* battery_get_info(void)
{
  char buffer[BUFFER_SIZE];
  char* uevent = NULL;
  bool found = false;

  // Search for an active battery interface (typically BAT0 to BAT5)
  for (int i = 0; i < 5; i++) {
    snprintf(buffer, sizeof(buffer), "/sys/class/power_supply/BAT%d/uevent", i);
    uevent = file_read_stripped(buffer, "=", false);
    
    if (uevent != NULL) {
      found = true;
      break; 
    }
  }

  if (found == false) {
    return NULL;
  }

  BATTERY* battery = calloc(1, sizeof(*battery));
  if (battery == NULL) {
    free(uevent);
    return NULL;
  }

  // Parse numeric values and convert from micro-units to standard units
  char* val = NULL;
  
  val = str_find_value(uevent, "POWER_SUPPLY_CAPACITY", "\n");
  if (val != NULL) {
    battery->capacity = atoi(val);
    free(val);
  }

  val = str_find_value(uevent, "POWER_SUPPLY_VOLTAGE_MIN_DESIGN", "\n");
  if (val != NULL) {
    battery->voltage_min_design = atof(val) / SYSFS_UNIT_CONVERSION;
    free(val);
  }

  val = str_find_value(uevent, "POWER_SUPPLY_VOLTAGE_NOW", "\n");
  if (val != NULL) {
    battery->voltage_now = atof(val) / SYSFS_UNIT_CONVERSION;
    free(val);
  }

  val = str_find_value(uevent, "POWER_SUPPLY_ENERGY_FULL_DESIGN", "\n");
  if (val != NULL) {
    battery->energy_full_design = atof(val) / SYSFS_UNIT_CONVERSION;
    free(val);
  }

  val = str_find_value(uevent, "POWER_SUPPLY_ENERGY_FULL", "\n");
  if (val != NULL) {
    battery->energy_full = atof(val) / SYSFS_UNIT_CONVERSION;
    free(val);
  }

  val = str_find_value(uevent, "POWER_SUPPLY_ENERGY_NOW", "\n");
  if (val != NULL) {
    battery->energy_now = atof(val) / SYSFS_UNIT_CONVERSION;
    free(val);
  }

  // Parse string values for vendor and state
  battery->supply_name    = str_find_value(uevent, "POWER_SUPPLY_NAME", "\n");
  battery->supply_type    = str_find_value(uevent, "POWER_SUPPLY_TYPE", "\n");
  battery->capacity_level = str_find_value(uevent, "POWER_SUPPLY_CAPACITY_LEVEL", "\n");
  battery->status         = str_find_value(uevent, "POWER_SUPPLY_STATUS", "\n");
  battery->technology     = str_find_value(uevent, "POWER_SUPPLY_TECHNOLOGY", "\n");
  battery->model_name     = str_find_value(uevent, "POWER_SUPPLY_MODEL_NAME", "\n");
  battery->manufacturer   = str_find_value(uevent, "POWER_SUPPLY_MANUFACTURER", "\n");
  battery->serial         = str_find_value(uevent, "POWER_SUPPLY_SERIAL_NUMBER", "\n");

  free(uevent);
  return battery;
}

/**
 * Deep-frees a BATTERY structure and its internal dynamically allocated strings.
 * @param battery Pointer to the structure to free.
 */
void free_battery(BATTERY* battery)
{
  if (battery == NULL) {
    return;
  }

  free(battery->supply_name);
  free(battery->supply_type);
  free(battery->capacity_level);
  free(battery->status);
  free(battery->technology);
  free(battery->model_name);
  free(battery->manufacturer);
  free(battery->serial);
  free(battery);
}

/**
 * Converts a BATTERY structure into a cJSON object for serialization.
 * @param battery Pointer to the BATTERY structure.
 * @return Pointer to a cJSON object (caller must delete).
 */
cJSON* battery_to_json_obj(const BATTERY* battery)
{
  cJSON* obj = cJSON_CreateObject();
  if (battery == NULL) {
    return obj;
  }

  // Numeric Metrics
  cJSON_AddNumberToObject(obj, "capacity_percent", battery->capacity);
  cJSON_AddNumberToObject(obj, "voltage_now", battery->voltage_now);
  cJSON_AddNumberToObject(obj, "voltage_min_design", battery->voltage_min_design);
  cJSON_AddNumberToObject(obj, "energy_now", battery->energy_now);
  cJSON_AddNumberToObject(obj, "energy_full", battery->energy_full);
  cJSON_AddNumberToObject(obj, "energy_full_design", battery->energy_full_design);

  // Vendor & State Strings
  cJSON_AddStringToObject(obj, "name", STR_OR_UNK(battery->supply_name));
  cJSON_AddStringToObject(obj, "type", STR_OR_UNK(battery->supply_type));
  cJSON_AddStringToObject(obj, "status", STR_OR_UNK(battery->status));
  cJSON_AddStringToObject(obj, "capacity_level", STR_OR_UNK(battery->capacity_level));
  cJSON_AddStringToObject(obj, "technology", STR_OR_UNK(battery->technology));
  cJSON_AddStringToObject(obj, "model_name", STR_OR_UNK(battery->model_name));
  cJSON_AddStringToObject(obj, "manufacturer", STR_OR_UNK(battery->manufacturer));
  cJSON_AddStringToObject(obj, "serial_number", STR_OR_UNK(battery->serial));

  return obj;
}

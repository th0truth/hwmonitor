#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "battery.h"

BAT *getBATTERYinfo()
{
  BAT *bat = malloc(sizeof *bat); 
  if (bat == NULL) {
    return NULL;
  }

  char buff[BUFF_SIZE];
  for (int i = 0; ; i++)
  {
    snprintf(buff, BUFF_SIZE, "/sys/class/power_supply/BAT%d/uevent", i);
    char *uevent = read_file(buff, "=", 0);
    if (i >= 5) {
      return NULL;
    } else if (uevent == NULL){
      continue;
    } 

    // Gather battery information
    bat->capacity           = atoi(findstr(uevent, "POWER_SUPPLY_CAPACITY", "\n"));
    bat->voltage_min_design = atof(findstr(uevent, "POWER_SUPPLY_VOLTAGE_MIN_DESIGN", "\n"))  / Watt;
    bat->voltage_now        = atof(findstr(uevent, "POWER_SUPPLY_VOLTAGE_NOW", "\n"))         / Watt;
    bat->energy_full_design = atof(findstr(uevent, "POWER_SUPPLY_ENERGY_FULL_DESIGN", "\n"))  / Watt;
    bat->energy_full        = atof(findstr(uevent, "POWER_SUPPLY_ENERGY_FULL", "\n"))         / Watt;
    bat->energy_now         = atof(findstr(uevent, "POWER_SUPPLY_ENERGY_NOW", "\n"))          / Watt;
    bat->supply_name        = findstr(uevent, "POWER_SUPPLY_NAME", "\n");
    bat->supply_type        = findstr(uevent, "POWER_SUPPLY_TYPE", "\n");
    bat->capacity_level     = findstr(uevent, "POWER_SUPPLY_CAPACITY_LEVEL", "\n");
    bat->status             = findstr(uevent, "POWER_SUPPLY_STATUS", "\n");
    bat->technology         = findstr(uevent, "POWER_SUPPLY_TECHNOLOGY", "\n");
    bat->model_name         = findstr(uevent, "POWER_SUPPLY_MODEL_NAME", "\n");
    bat->manufacturer       = findstr(uevent, "POWER_SUPPLY_MANUFACTURER", "\n");
    bat->serial             = findstr(uevent, "POWER_SUPPLY_SERIAL_NUMBER", "\n");
    
    free(uevent);
    return bat;
  }
}

void free_battery(BAT *bat)
{
  if (!bat) return;
  free(bat->supply_name);
  free(bat->supply_type);
  free(bat->capacity_level);
  free(bat->status);
  free(bat->technology);
  free(bat->model_name);
  free(bat->manufacturer);
  free(bat->serial);
  free(bat);
}
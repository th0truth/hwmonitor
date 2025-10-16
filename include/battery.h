#pragma once

#include <inttypes.h>
#include "platform.h"

#if defined(HWMONITOR_UNIX)
#define BUFF_SIZE 128
#define Watt 1000000
 
typedef struct {
  char *supply_name;
  char *supply_type; 
  char *capacity_level; 
  char *status;
  char *technology;
  char *model_name;
  char *manufacturer;
  char *serial;
  float voltage_min_design;
  float voltage_now;
  float energy_full_design;
  float energy_full;
  float energy_now;
  uint16_t capacity;
} BAT; 

BAT *getBATTERYinfo();
void free_battery(BAT *bat);
#endif
#pragma once

#include <inttypes.h>
#include "platform.h"

#if defined(HWMONITOR_UNIX)
#define BUFF_SIZE 128

typedef struct {
  uint16_t capacity;
  uint64_t voltage_min;
  uint64_t voltage_now;
  char *supply_name;
  char *supply_type; 
  char *capacity_level; 
  char *status;
  char *technology;
  char *model_name;
  char *manufacturer;
  char *serial;
} BAT; 

BAT *getBatteryinfo();
#endif
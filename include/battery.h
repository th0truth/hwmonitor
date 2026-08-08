#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <inttypes.h>
#include <cJSON.h>

typedef struct {
    uint16_t capacity;
    float voltage_min_design;
    float voltage_now;
    float energy_full_design;
    float energy_full;
    float energy_now;
    char *supply_name;
    char *supply_type;
    char *capacity_level;
    char *status;
    char *technology;
    char *model_name;
    char *manufacturer;
    char *serial;
} BATTERY;

BATTERY *battery_get_info(void);
void free_battery(BATTERY *battery);
cJSON *battery_to_json_obj(const BATTERY *battery);

#ifdef __cplusplus
}
#endif /* __cplusplus */

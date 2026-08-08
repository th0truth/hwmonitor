#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <cJSON.h>

typedef struct {
    char *sys_vendor;
    char *product_name;
    char *product_family;
    char *product_sku;
    char *board_vendor;
    char *board_name;
    char *serial;
} MAINBOARD;

MAINBOARD *mainboard_get_info(void);
void free_mainboard(MAINBOARD *mainboard);
cJSON *mainboard_to_json_obj(const MAINBOARD *mainboard);

#ifdef __cplusplus
}
#endif /* __cplusplus */

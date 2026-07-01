/**
 * @file mainboard.c
 * @brief Hardware discovery and parsing logic for system/mainboard DMI.
 */

#include "base.h"
#include "file.h"
#include "io.h"
#include "mainboard.h"

MAINBOARD* mainboard_get_info(void)
{
  MAINBOARD* mainboard = calloc(1, sizeof(MAINBOARD));
  if (!mainboard)
    return NULL;
  
  // Note: some of these (especially serial) may require root permissions.
  // file_read_stripped will safely return NULL if permission is denied.
  mainboard->sys_vendor     = file_read_stripped("/sys/class/dmi/id/sys_vendor", "\n", false);
  mainboard->product_name   = file_read_stripped("/sys/class/dmi/id/product_name", "\n", false);
  mainboard->product_family = file_read_stripped("/sys/class/dmi/id/product_family", "\n", false);
  mainboard->product_sku    = file_read_stripped("/sys/class/dmi/id/product_sku", "\n", false);
  mainboard->board_vendor   = file_read_stripped("/sys/class/dmi/id/board_vendor", "\n", false);
  mainboard->board_name     = file_read_stripped("/sys/class/dmi/id/board_name", "\n", false);
  mainboard->serial         = file_read_stripped("/sys/class/dmi/id/product_serial", "\n", false);

  return mainboard;
}

void free_mainboard(MAINBOARD* mainboard)
{
  if (!mainboard)
    return;
  free(mainboard->sys_vendor);
  free(mainboard->product_name);
  free(mainboard->product_family);
  free(mainboard->product_sku);
  free(mainboard->board_vendor);
  free(mainboard->board_name);
  free(mainboard->serial);
  free(mainboard);
}

cJSON* mainboard_to_json_obj(const MAINBOARD* mainboard)
{
  cJSON* obj = cJSON_CreateObject();
  if (!mainboard)
    return obj;

  cJSON_AddStringToObject(obj, "sys_vendor", STR_OR_UNK(mainboard->sys_vendor));
  cJSON_AddStringToObject(obj, "product_name", STR_OR_UNK(mainboard->product_name));
  cJSON_AddStringToObject(obj, "product_family", STR_OR_UNK(mainboard->product_family));
  cJSON_AddStringToObject(obj, "product_sku", STR_OR_UNK(mainboard->product_sku));
  cJSON_AddStringToObject(obj, "board_vendor", STR_OR_UNK(mainboard->board_vendor));
  cJSON_AddStringToObject(obj, "board_name", STR_OR_UNK(mainboard->board_name));
  cJSON_AddStringToObject(obj, "serial", STR_OR_UNK(mainboard->serial));

  return obj;
}

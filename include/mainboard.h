/**
 * @file mainboard.h
 * @brief Header file for mainboard/system DMI discovery.
 */

#pragma once

#include <cJSON.h>

/**
 * @struct MAINBOARD
 * @brief Structure to hold system and motherboard DMI information.
 */
typedef struct {
  char* sys_vendor;
  char* product_name;
  char* product_family;
  char* product_sku;
  char* board_vendor;
  char* board_name;
  char* serial;
} MAINBOARD;

/**
 * @brief Discovers and parses Mainboard/System information from sysfs DMI data.
 * @return Pointer to a newly allocated MAINBOARD struct, or NULL on failure.
 */
MAINBOARD* mainboard_get_info(void);

/**
 * @brief Deep-frees a MAINBOARD structure and its internal strings.
 * @param mainboard Pointer to the MAINBOARD structure to free.
 */
void free_mainboard(MAINBOARD* mainboard);

/**
 * @brief Converts a MAINBOARD structure to a cJSON object.
 * @param mainboard Pointer to the MAINBOARD structure.
 * @return Pointer to a cJSON object (caller must delete).
 */
cJSON* mainboard_to_json_obj(const MAINBOARD* mainboard);

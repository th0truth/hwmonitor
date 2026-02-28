/**
 * @file os.h
 * @brief Header file for os module.
 */

#pragma once

#include "os.h"
#include <cJSON.h>

/**
 * @struct OS
 * @brief Structure to hold operating system release information.
 */
typedef struct {
  char* name;
  char* version_id;
  char* build_id;
  char* release_type;
  char* id;
  char* de;
  char* de_id;
} OS;


/**
 * @brief Discovers and parses OS information (typically from /etc/os-release).
 * @return Pointer to a newly allocated OS struct, or NULL on failure.
 */
OS* os_get_info(void);
/**
 * @brief Deep-frees an OS structure and its internal strings.
 * @param os Pointer to the OS structure to free.
 */
void free_os(OS* os);
/**
 * @brief Converts an OS structure to a cJSON object.
 * @param os Pointer to the OS structure.
 * @return Pointer to a cJSON object (caller must delete).
 */
cJSON* os_to_json_obj(const OS* os);

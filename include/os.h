#pragma once

#include "os.h"

typedef struct {
  char* name;
  char* version_id;
  char* build_id;
  char* release_type;
  char* id;
  char* de;
  char* de_id;
} OS;


OS* os_get_info(void);
void free_os(OS* os);
#include "base.h"
#include "file.h"
#include "io.h"

#include "os.h"

OS* os_get_info(void)
{
  OS* os = calloc(1, sizeof(OS));
  if (!os)
    return NULL;
  
  char* os_info = file_read_stripped("/etc/os-release", "\"", false);
  if (!os_info) {
    free(os);
    return NULL;
  }

  os->name          = str_find_value(os_info, "NAME=", "\n");
  os->version_id    = str_find_value(os_info, "VERSION_ID=", "\n");
  os->build_id      = str_find_value(os_info, "BUILD_ID=", "\n");
  os->release_type  = str_find_value(os_info, "RELEASE_TYPE=", "\n");
  os->id            = str_find_value(os_info, "ID=", "\n");
  os->de            = str_find_value(os_info, "VARIANT=", "\n");
  os->de_id         = str_find_value(os_info, "VARIANT_ID=", "\n");

  free(os_info);

  return os;
}

void free_os(OS* os)
{
  if (!os)
    return;
  free(os->name);
  free(os->version_id);
  free(os->release_type);
  free(os->id);
  free(os->de);
  free(os->de_id);
  free(os);
}
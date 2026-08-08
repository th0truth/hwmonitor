#include "base.h"
#include "file.h"
#include "io.h"
#include "os.h"

OS *
os_get_info(void)
{
  OS *os = calloc(1, sizeof(OS));
  if (os == NULL) {
    return NULL;
  }

  /* Parse /etc/os-release, stripping quote characters for clean values */
  char *os_info = file_read_stripped("/etc/os-release", "\"", false);
  if (os_info == NULL) {
    free(os);
    return NULL;
  }

  /* Extract relevant key-value pairs */
  os->name         = str_find_value(os_info, "NAME=", "\n");
  os->version_id   = str_find_value(os_info, "VERSION_ID=", "\n");
  os->build_id     = str_find_value(os_info, "BUILD_ID=", "\n");
  os->release_type = str_find_value(os_info, "RELEASE_TYPE=", "\n");
  os->id           = str_find_value(os_info, "ID=", "\n");
  os->de           = str_find_value(os_info, "VARIANT=", "\n");
  os->de_id        = str_find_value(os_info, "VARIANT_ID=", "\n");

  free(os_info);
  return os;
}

void
free_os(OS *os)
{
  if (os == NULL) {
    return;
  }

  free(os->name);
  free(os->version_id);
  free(os->build_id);
  free(os->release_type);
  free(os->id);
  free(os->de);
  free(os->de_id);
  free(os);
}

cJSON *
os_to_json_obj(const OS *os)
{
  cJSON *obj = cJSON_CreateObject();
  if (os == NULL) {
    return obj;
  }

  cJSON_AddStringToObject(obj, "name", STR_OR_UNK(os->name));
  cJSON_AddStringToObject(obj, "version_id", STR_OR_UNK(os->version_id));
  cJSON_AddStringToObject(obj, "build_id", STR_OR_UNK(os->build_id));
  cJSON_AddStringToObject(obj, "release_type", STR_OR_UNK(os->release_type));
  cJSON_AddStringToObject(obj, "id", STR_OR_UNK(os->id));
  cJSON_AddStringToObject(obj, "de_variant", STR_OR_UNK(os->de));
  cJSON_AddStringToObject(obj, "de_id", STR_OR_UNK(os->de_id));

  return obj;
}

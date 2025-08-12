#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#include "utils.h"
#include "mobo.h"

MOBO *getMOBOinfo()
{
  MOBO *motherboard = (MOBO*)malloc(sizeof(MOBO));

  // Get motherboard manufacturer
  char *manufacturer = read_file("/sys/class/dmi/id/board_vendor", "\n");
  if (manufacturer == NULL) {
    free(motherboard);
    return NULL;
  }
  motherboard->manufacturer = malloc(strlen(manufacturer));
  strncpy(motherboard->manufacturer, manufacturer, (strlen(manufacturer) + 1));
  free(manufacturer);

  // Get motherboard model
  char *model = read_file("/sys/class/dmi/id/board_name", "\n");
  if (model == NULL) {
    free(motherboard);
    return NULL;
  }
  motherboard->model = malloc(strlen(model));
  strncpy(motherboard->model, model, (strlen(model) + 1));
  free(model);

  // Get motherboard version
  char *version = read_file("/sys/class/dmi/id/board_version", "\n");
  if (version == NULL) {
    free(motherboard);
    return NULL;
  }
  motherboard->version = malloc(strlen(version));
  strncpy(motherboard->version, version, (strlen(version) + 1));
  free(version);

  return motherboard;
}
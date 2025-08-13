#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#include "utils.h"
#include "mobo.h"

MOBO *getMOBOinfo()
{
  MOBO *motherboard = malloc(sizeof *motherboard);
  if (motherboard == NULL) {
    fprintf(stderr, "Memory allocation filed.\n");
    return NULL;
  }

  // Get motherboard manufacturer
  char *manufacturer = read_file("/sys/class/dmi/id/board_vendor", "\n", 1);
  if (manufacturer == NULL) {
    free(motherboard);
    return NULL;
  }
  motherboard->manufacturer = malloc(strlen(manufacturer));
  strncpy(motherboard->manufacturer, manufacturer, (strlen(manufacturer) + 1));
  free(manufacturer);

  // Get motherboard model
  char *model = read_file("/sys/class/dmi/id/board_name", "\n", 1);
  if (model == NULL) {
    free(motherboard);
    return NULL;
  }
  motherboard->model = malloc(strlen(model));
  strncpy(motherboard->model, model, (strlen(model) + 1));
  free(model);

  // Get motherboard version
  char *version = read_file("/sys/class/dmi/id/board_version", "\n", 1);
  if (version == NULL) {
    free(motherboard);
    return NULL;
  }
  motherboard->version = malloc(strlen(version));
  strncpy(motherboard->version, version, (strlen(version) + 1));
  free(version);

  return motherboard;
}

BIOS *getBIOSinfo()
{
  BIOS *bios = malloc(sizeof *bios);
  if (bios == NULL) {
    fprintf(stderr, "Memory allocation filed.\n");
    return NULL;
  }

  // Get BIOS vendor
  char *vendor = read_file("/sys/class/dmi/id/bios_vendor", "\n", 1);
  if (vendor == NULL) {
    free(bios);
    return NULL;
  }
  bios->vendor = malloc(strlen(vendor));
  strncpy(bios->vendor, vendor, (strlen(vendor) + 1));
  free(vendor);

  // Get BIOS version
  char *version = read_file("/sys/class/dmi/id/bios_version", "\n", 1);
  if (version == NULL) {
    free(bios);
    return NULL;
  }
  bios->version = malloc(strlen(version));
  strncpy(bios->version, version, (strlen(version) + 1));
  free(version);

  // Get BIOS release date
  char *date = read_file("/sys/class/dmi/id/bios_date", "\n", 1);
  if (date == NULL) {
    free(bios);
    return NULL;
  }
  bios->release_date = malloc(strlen(date));
  strncpy(bios->release_date, date, (strlen(date) + 1));
  free(date);

  return bios;
}
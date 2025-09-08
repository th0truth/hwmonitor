#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"
#include "motherboard.h"

Motherboard *getMotherboardInfo()
{
  Motherboard *mobo = malloc(sizeof *mobo);
  if (mobo == NULL) {
    fprintf(stderr, "Memory allocation filed.\n");
    return NULL;
  }

  // Gather motheboard information
  mobo->manufacturer = read_file("/sys/class/dmi/id/board_vendor", "\n", 1);
  mobo->model        = read_file("/sys/class/dmi/id/board_name", "\n", 1);
  mobo->version      = read_file("/sys/class/dmi/id/board_version", "\n", 1);

  return mobo;
}

void free_motherboard(Motherboard *mobo)
{
  if (!mobo) return;
  free(mobo->manufacturer);
  free(mobo->model);
  free(mobo->version);
  free(mobo);
}
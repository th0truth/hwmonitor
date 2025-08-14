#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#include "utils.h"
#include "mobo.h"

MOBO *getMOBOinfo()
{
  MOBO *mobo = malloc(sizeof *mobo);
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

BIOS *getBIOSinfo()
{
  BIOS *bios = malloc(sizeof *bios);
  if (bios == NULL) {
    fprintf(stderr, "Memory allocation filed.\n");
    return NULL;
  }

  // Gather BIOS information
  bios->vendor  = read_file("/sys/class/dmi/id/bios_vendor", "\n", 1);
  bios->version = read_file("/sys/class/dmi/id/bios_version", "\n", 1);
  bios->date    = read_file("/sys/class/dmi/id/bios_date", "\n", 1);

  return bios;
}

void free_mobo(MOBO *mobo)
{
  if (!mobo) return;
  free(mobo->manufacturer);
  free(mobo->model);
  free(mobo->version);
  free(mobo);
}

void free_bios(BIOS *bios)
{
  if (!bios) return;
  free(bios->vendor);
  free(bios->version);
  free(bios->date);
  free(bios);
}
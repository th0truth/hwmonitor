#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"
#include "bios.h"

BIOS *getBIOSinfo()
{
  BIOS *bios = malloc(sizeof *bios);
  if (!bios) {
    fprintf(stderr, "Memory allocation filed.\n");
    return NULL;
  }

  // Gather BIOS information
  bios->vendor  = read_file("/sys/class/dmi/id/bios_vendor", "\n", 1);
  bios->version = read_file("/sys/class/dmi/id/bios_version", "\n", 1);
  bios->date    = read_file("/sys/class/dmi/id/bios_date", "\n", 1);

  return bios;
}

void free_bios(BIOS *bios)
{
  if (!bios) return;
  free(bios->vendor);
  free(bios->version);
  free(bios->date);
  free(bios);
}
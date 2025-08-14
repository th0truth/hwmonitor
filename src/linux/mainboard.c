#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "utils.h"
#include "mainboard.h"

MAINBOARD *getMainboardInfo()
{
  MAINBOARD *mainboard = malloc(sizeof *mainboard);
  if (mainboard == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    return NULL;
  }

  // Gather mainboard information
  mainboard->name   = read_file("/sys/class/dmi/id/product_name", "\n", 1);
  mainboard->family = read_file("/sys/class/dmi/id/product_family", "\n", 1);
  mainboard->serial = read_file("/sys/class/dmi/id/product_serial", "\n", 1);
  mainboard->sku    = read_file("/sys/class/dmi/id/product_sku", "\n", 1);

  return mainboard;
};
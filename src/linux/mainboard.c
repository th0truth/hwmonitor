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

  // Get product name  
  char *name = read_file("/sys/class/dmi/id/product_name", "\n");
  if (name == NULL) {
    free(mainboard);
    return NULL;
  } 
  mainboard->name = malloc(strlen(name));
  strncpy(mainboard->name, name, (strlen(name) + 1));
  free(name);

  // Get product family  
  char *family = read_file("/sys/class/dmi/id/product_family", "\n");
  if (family == NULL) {
    free(mainboard);
    return NULL;
  } 
  mainboard->family = malloc(strlen(family));
  strncpy(mainboard->family, family, (strlen(family) + 1));
  free(family);

  // Get product serial
  char *serial = read_file("/sys/class/dmi/id/product_serial", "\n");
  if (serial == NULL) {
    free(mainboard);
    return NULL;
  } 
  mainboard->serial = malloc(strlen(serial));
  strncpy(mainboard->serial, serial, (strlen(serial) + 1));
  free(serial);

  // Get product sku
  char *sku = read_file("/sys/class/dmi/id/product_sku", "\n");
  if (sku == NULL) {
    free(mainboard);
    return NULL;
  } 
  mainboard->sku = malloc(strlen(sku));
  strncpy(mainboard->sku, sku, (strlen(sku) + 1));
  free(sku);


  return mainboard;
};
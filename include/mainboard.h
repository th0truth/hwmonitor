#pragma once

#ifndef MAINBOARD_H
#define MAINBOARD_H

typedef struct {
  char *name;
  char *family;
  char *serial;
  char *sku;
} MAINBOARD;

MAINBOARD *getMainboardInfo();
void free_mainboard(MAINBOARD *mb);
#endif
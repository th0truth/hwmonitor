#pragma once

#ifndef MOBO_H
#define MOBO_H

typedef struct {
  char *manufacturer;
  char *model;
  char *version;
} MOBO;

typedef struct {
  char *vendor;
  char *version;
  char *date;
} BIOS;

MOBO *getMOBOinfo();
BIOS *getBIOSinfo();
void free_mobo(MOBO *mobo);
void free_bios(BIOS *bios);
#endif
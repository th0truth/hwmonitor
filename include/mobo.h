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
  char *release_date;
} BIOS;

MOBO *getMOBOinfo();
BIOS *getBIOSinfo();
#endif
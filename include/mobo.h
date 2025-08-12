#pragma once

#ifndef MOBO_H
#define MOBO_H

typedef struct {
  char *manufacturer;
  char *model;
  char *version;
} MOBO;

MOBO *getMOBOinfo();
#endif
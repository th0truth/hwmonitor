#pragma once

#ifndef OS_H
#define OS_H

typedef struct {
  char *NAME;
  int16_t VERSION_ID;
  char *RELEASE_TYPE;
  char *ID;
  char *ID_LIKE;
  char *DE;
  char *DE_ID;
} OS;

OS *getOSinfo();
#endif
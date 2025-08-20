#pragma once

#include <stdint.h>

#include "platform.h"

#if defined(HWMONITOR_UNIX)
char *read_file(char *filename, char *rmch, int p_err);
char *findstr(const char *src, const char *srch, const char *delim);
char *trim(char *str);
float calcsz(char *unit, uint64_t size);
#endif
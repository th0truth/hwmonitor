#pragma once

#include <stdint.h>
#ifndef UTILS_H
#define UTILS_H
char *read_file(char *filename, char *rmch);
char *findstr(const char *src, const char *srch, const char *delim);
char *trim(char *str);
float calcsz(char *unit, uint64_t size);
#endif
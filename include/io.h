#pragma once

#include "base.h"

/**
 * Searches a source string for a key and returns the associated value.
 * @return Dynamically allocated string (caller must free).
 */
char* str_find_value(const char* source, const char* search, const char* delim);

/**
 * Searches a source string for a key and returns the value as a double.
 * Handles memory allocation and cleanup internally.
 * @return Value as a double, or 0.0 if not found.
 */
double str_parse_value(const char* source, const char* search, const char* delim);

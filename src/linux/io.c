#include "base.h"
#include "io.h"

/**
 * Searches a source string for a key and returns the associated value.
 * Trims leading and trailing whitespace.
 * @param source The source string to search (e.g., contents of /proc/cpuinfo).
 * @param search The key string to find (e.g., "vendor_id").
 * @param delim A set of delimiter characters to find the end of the value (e.g., "\n").
 * @return Dynamically allocated value string, or NULL if the key or value is not found.
 */
char* str_find_value(const char* source, const char* search, const char* delim)
{
  if (source == NULL || search == NULL || delim == NULL) {
    return NULL;
  }

  /* Locate the key in the source string */
  const char* start = strstr(source, search);
  if (start == NULL) {
    return NULL;
  }

  /* Skip past the length of the key itself */
  start += strlen(search);

  /* Skip leading separators and whitespaces to reach the start of the value */
  while (*start && (isspace((unsigned char)*start) || *start == ':')) {
    start++;
  }

  /* Find the end of the value based on the provided delimiters */
  const char* end = start;
  while (*end && !strchr(delim, *end)) {
    end++;
  }

  /* Backtrack to trim any trailing whitespace */
  while (end > start && isspace((unsigned char)*(end - 1))) {
    end--;
  }

  uint64_t len = end - start;
  if (len == 0) {
    return NULL;
  }

  /* Allocate memory and copy the resulting value */
  char* value = malloc(len + 1);
  if (value == NULL) {
    return NULL;
  }

  memcpy(value, start, len);
  value[len] = '\0';

  return value;
}

/**
 * Searches a source string for a key and returns the value converted to double.
 * Automatically handles memory allocation and cleanup of the intermediate string.
 * @param source The source string to search.
 * @param search The key string to find.
 * @param delim A set of delimiter characters to find the end of the value.
 * @return The numeric value as a double, or 0.0 if not found.
 */
double str_parse_value(const char* source, const char* search, const char* delim)
{
  char* value = str_find_value(source, search, delim);
  if (value == NULL) {
    return 0.0;
  }

  double result = atof(value);
  free(value);
  
  return result;
}

#include "base.h"

/**
 * Searches a source string for a key and returns the associated value.
 * Trims leading and trailing whitespace.
 * @param source The source string to search.
 * @param search The key string to find.
 * @param delim A set of delimiter characters to find the end of the value.
 * @return Dynamically allocated value string, or NULL if not found.
 */
char* str_find_value(const char* source, const char* search, const char* delim)
{
  if (!source || !search || !delim) {
    return NULL;
  }

  /* Locate the key */
  const char* start = strstr(source, search);
  if (!start) {
    return NULL;
  }

  /* Skip past the key length */
  start += strlen(search);

  /* Skip leading separators and whitespaces */
  while (*start && (isspace((unsigned char)*start) || *start == ':')) {
    start++;
  }

  /* Find the end of the value based on delimiters */
  const char* end = start;
  while (*end && !strchr(delim, *end)) {
    end++;
  }

  /* Backtrack to trim trailing whitespace */
  while (end > start && isspace((unsigned char)*(end - 1))) {
    end--;
  }

  uint64_t len = end - start;
  if (!len) {
    return NULL;
  }

  /* Allocate and copy the result */
  char* value = malloc(len + 1);
  if (!value) {
    return NULL;
  }

  memcpy(value, start, len);
  value[len] = '\0';

  return value;
}

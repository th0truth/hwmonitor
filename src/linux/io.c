#include "base.h"
#include <ctype.h>

char* str_find_value(const char* source, const char* search, const char* delim)
{
	const char* start;
	const char* end;
	char* value;
	size_t len;

	if (!source || !search || !delim) {
		return NULL;
  }

	/* Locate the key */
	start = strstr(source, search);
	if (!start) {
		return NULL;
  }

	/* Skip past the key length */
	start += strlen(search);

	/* Skip leading separators and whitespaces */
	while (*start && (isspace((unsigned char)*start) || *start == ':')) {
    start++;
  }

	/* Find the end of the value */
	end = start;
	while (*end && !strchr(delim, *end)) {
		end++;
  }

	/* Backtrack to trim trailing whitespace */
	while (end > start && isspace((unsigned char)*(end - 1))) {
		end--;
  }

	len = (size_t)(end - start);
	if (!len) {
		return NULL;
  }

	/* Allocate and copy */
	value = malloc(len + 1);
	if (!value) {
		return NULL;
  }

	memcpy(value, start, len);
	value[len] = '\0';

	return value;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *findstr(const char *src, const char *srch, const char *delim) {
  char *str = strstr(src, srch);
  if (str == NULL) {
    fprintf(stderr, "Substring not found.");
    return NULL;
  }
  str += strlen(srch);
  str = strtok(str, delim);
  return str; 
}
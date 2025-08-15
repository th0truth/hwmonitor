#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>

// https://stackoverflow.com/a/122974
char *trim(char *str)
{
  size_t len = 0;
  char *frontp = str;
  char *endp = NULL; 

  if ( str == NULL ) { return NULL; }
  if ( str[0] == '\0' ) { return str; }
  
  len = strlen(str);
  endp = str + len;

  while( isspace((unsigned char) *frontp) ) { ++frontp; }
  if (endp != frontp)
  {
    while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
  }
    
  if(frontp != str && endp == frontp )
  {
    // Empty string
    *(isspace((unsigned char) *endp) ? str : (endp + 1)) = '\0';
  }
  
  else if ( str + len - 1 != endp ) *(endp + 1) = '\0';

  endp = str;
  if( frontp != str )
  {
    while( *frontp ) { *endp++ = *frontp++; } *endp = '\0';
  }
    
  return str;
}

char *findstr(const char *src, const char *srch, const char *delim)
{
  // Find the substring
  char *str = strstr(src, srch);
  if (str == NULL) {
    return NULL;
  }

  // Move past the search string
  size_t len = 0;
  str += strlen(srch);
  while (str[len] && !strchr(delim, str[len])) {
    len++;
  }

  // Remove whitespaces from string
  trim(str);
  
  // Allocate memory for the result
  char *result = (char*)malloc(sizeof(char) * len + 1);
  if (result == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }

  // Copy str to string using safe approach
  strncpy(result, str, len);
  result[len] = '\0';
  
  return result; 
}

// Convert size to appopriate unit
float calcsz(char *unit, uint64_t size)
{
  uint64_t multiplier = 1ULL << 60;
  const char *sizes[] = {"ZiB", "EiB", "PiB", "TiB", "GiB", "MiB", "KiB", "B"};
  for (int i = 0; i < (sizeof(sizes) / sizeof(*sizes)); i++, multiplier /= 1024) {
    if (strcmp(sizes[i], unit) == 0) {
      break;
    }
  }
  
  return size / (float)multiplier;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

char *findstr(const char *src, const char *srch, const char *delim) {
  // Find the substring
  char *str = strstr(src, srch);
  if (str == NULL) {
    fprintf(stderr, "Substring '%s' not found.\n", srch);
    return NULL;
  }

  // Move past the search string
  str += strlen(srch);
  size_t len = 0;
  while (str[len] && !strchr(delim, str[len])) {
    len++;
  }
  
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
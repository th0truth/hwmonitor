#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUFF_SIZE 1024

char *read_file(const char *filename)
{
  FILE *fptr = fopen(filename, "r");
  if ((ferror(fptr)) == 2) {
    fprintf(stderr, "Failed while opening file: %s", strerror(errno));
    exit(1);
  };

  // https://stackoverflow.com/a/31057315
  // Read character then store it in the buffer.
  size_t nch = 0, size = 10;
  char *buff = malloc(BUFF_SIZE);
  while (!feof(fptr)) {
    char c = fgetc(fptr);
    if (nch >= size-1) {
      size += 10;
      buff = realloc(buff, size);
      if (buff == NULL) {
        fprintf(stderr, "Out of memory");
        free(buff);
        exit(EXIT_FAILURE);
      }
    }
    if (c != '\t' && c != ':') {
      buff[nch++] = c;
    }
  }
  
  fclose(fptr);
  return buff;
}
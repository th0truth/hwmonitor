#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define BUFF_SIZE 1024

int chmatch(const char c, char *chars, int size) {
  for (int i = 0; i < size; i++)
  {
    if (c == chars[i]) {
      return 0;
    }
  }
  return 1;
}

char *read_file(const char *filename, char *rmch)
{
  FILE *fptr = fopen(filename, "r");
  if (fptr == NULL) {
    fprintf(stderr, "File '%s' openning failed: %s\n", filename, strerror(errno));
    return NULL;
  };

  unsigned rmch_sz = sizeof(rmch) / sizeof(rmch[0]);

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

    if (chmatch(c, rmch, rmch_sz)) {
      buff[nch++] = c;
    }
  }
  buff[nch] = '\0';
  
  fclose(fptr);
  return buff;
}
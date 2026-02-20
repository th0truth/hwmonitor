#include "base.h"

#define BUFFER_SIZE 1024

char* file_read_stripped(const char* filename, const char* exclude, bool verbose)
{
  FILE* fp = fopen(filename, "r");
  if (!fp) {
    if (verbose) {
      fprintf(stderr, "error: failed to open '%s': %s\n", filename, strerror(errno));
    }
    return NULL;
  }

  char* buffer = malloc(BUFFER_SIZE);
  if (!buffer) {
    if (verbose) {
      fprintf(stderr, "error: failed to allocate memory: %s", strerror(errno));
    }
    return NULL;
  }

  int c;
  int n = 0;
  uint64_t capacity = BUFFER_SIZE;
  while ((c = fgetc(fp)) != EOF) {
    if (n + 1 >= capacity) {
      capacity *= 2;
      char* line_buffer = realloc(buffer, capacity);
      if (!line_buffer) {
        free(buffer);
        fclose(fp);
        return NULL;
      }
      buffer = line_buffer;
    }

    if (!exclude || !strchr(exclude, c)) {
      buffer[n++] = c;
    }
  }

  buffer[n] = '\0';
  fclose(fp);
  
  return buffer;
}

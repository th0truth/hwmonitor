#include "base.h"

#define FILE_READ_BUFFER 4096

char* file_read_stripped(const char* filename, const char* exclude, bool verbose)
{
  FILE* fp = fopen(filename, "r");
  if (!fp) {
    if (verbose) {
      fprintf(stderr, "error: failed to open '%s': %s\n", filename, strerror(errno));
    }
    return NULL;
  }

  char* buffer = malloc(FILE_READ_BUFFER);
  if (!buffer) {
    if (verbose) {
      fprintf(stderr, "error: failed to allocate memory: %s", strerror(errno));
    }
    return NULL;
  }

  int c;
  int n = 0;
  uint64_t capacity = FILE_READ_BUFFER;
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

bool file_write_string(const char* filename, const char* data)
{
  if (filename == NULL || data == NULL) {
    return false;
  }

  FILE* fp = fopen(filename, "w");
  if (fp == NULL) {
    fprintf(stderr, "error: failed to open '%s' for writing: %s\n", filename, strerror(errno));
    return false;
  }
  
  if (fputs(data, fp) == EOF) {
    fprintf(stderr, "error: failed to write to '%s': %s\n", filename, strerror(errno));
    return false;
  }

  fclose(fp);

  return true;
}

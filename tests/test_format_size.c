#include <assert.h>
#include <string.h>
#include "util.h"

int main(void)
{
  char buffer[32];

  format_size("GiB", 8ULL * 1024ULL * 1024ULL * 1024ULL, buffer, sizeof(buffer));
  assert(strcmp(buffer, "8.00 GiB") == 0);

  format_size("MiB", 512ULL * 1024ULL * 1024ULL, buffer, sizeof(buffer));
  assert(strcmp(buffer, "512.00 MiB") == 0);

  return 0;
}
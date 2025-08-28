#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "os.h"

OS *getOSinfo()
{
  OS *os = malloc(sizeof *os);
  if (os == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    return NULL;
  }

  char *osinfo = read_file("/etc/os-release", "\"", 1);
  if (osinfo == NULL) {
    free(os);
    return NULL;
  }
  
  // Gather OS information
  os->NAME         = findstr(osinfo, "NAME=", "\n");
  os->VERSION_ID   = findstr(osinfo, "VERSION_ID=", "\n");
  os->RELEASE_TYPE = findstr(osinfo, "RELEASE_TYPE=", "\n");
  os->ID           = findstr(osinfo, "ID=", "\n");
  os->DE           = findstr(osinfo, "VARIANT=", "\n");
  os->DE_ID        = findstr(osinfo, "VARIANT_ID=", "\n"); 

  free(osinfo);  
  return os;
}

void free_os(OS *os)
{
  if (!os) return;
  free(os->NAME);
  free(os->VERSION_ID);
  free(os->RELEASE_TYPE);
  free(os->ID);
  free(os->DE);
  free(os->DE_ID);
  free(os);
}
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "os.h"

OS *getOSinfo()
{
  OS *os = (OS*)malloc(sizeof(OS));
  if (os == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    return NULL;
  }

  char rmch[] = {'"'};
  char *osinfo = read_file("/etc/os-release", rmch);
  if (osinfo == NULL) {
    free(os);
    return NULL;
  }
  
  os->NAME =          findstr(osinfo, "NAME=", "\n");
  os->VERSION_ID =    atoi(findstr(osinfo, "VERSION_ID=", "\n"));
  os->RELEASE_TYPE =  findstr(osinfo, "RELEASE_TYPE=", "\n");
  os->ID =            findstr(osinfo, "ID=", "\n");
  os->DE =            findstr(osinfo, "VARIANT=", "\n");
  os->DE_ID =         findstr(osinfo, "VARIANT_ID=", "\n"); 

  free(osinfo);  
  return os;
};
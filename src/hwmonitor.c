#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include "hwmonitor.h"

int main(int argc, char *argv[])
{
  int opt;
  static struct option long_options[] = {
    {"help",        no_argument,  NULL, 'h'},
    {"cpu",         no_argument,  NULL, 'c'},
    {"gpu",         no_argument,  NULL, 'g'},
    {"ram",         no_argument,  NULL, 'r'},
    {"bat",         no_argument,  NULL, 'b'},
    {"network",     required_argument,  NULL, 'n'},
    {"motherboard", no_argument,  NULL, 'm'},
    {"bios",        no_argument,  NULL, 'B'},
    {"mainboard",   no_argument,  NULL, 'M'},
    {"os",          no_argument,  NULL, 's'},
    {NULL,          0,            NULL,  0}
  };
  
  int option_index = 0;
  while ((opt = getopt_long(argc, argv, "hcgrbn:MBHs", long_options, &option_index)) != -1) {
    switch (opt) {
      case 'h':
        print_Help(argv[0]);
        return 0;
      case 'c':
        print_CPU();
        break;
      case 'g':
        print_GPU();
        break;
      case 'r':
        print_RAM();
        break;
      case 'b':
        print_BAT();
        break;
      case 'n':
        print_NET(optarg);
        break;
      case 'm':
        print_Motherboard();
        break;
      case 'B':
        print_BIOS();
        break;
      case 'M':
        print_Mainboard();
        break;
      case 's':
        print_OS();
        break;
      
        default:
          print_Help(argv[0]);
          exit(EXIT_FAILURE);
    }
  }

  return 0;
}
#include "base.h"
#include <getopt.h>

#include "cpu.h"

static struct option options[] = {
  {"help",        no_argument,  NULL, 'h'},
  {"cpu",         no_argument,  NULL, 'c'},
  {"gpu",         no_argument,  NULL, 'g'},
  {"ram",         no_argument,  NULL, 'r'},
  {"bat",         no_argument,  NULL, 'b'},
  {"network",     required_argument,  NULL, 'n'},
  {"motherboard", no_argument,  NULL, 'm'},
  {"bios",        no_argument,  NULL, 'B'},
  {"mainboard",   no_argument,  NULL, 'M'},
  {"os",          no_argument,  NULL, 'o'},
  {"storage",     no_argument,  NULL, 's'},
  {NULL,          0,            NULL,  0}
};

void print_usage(const char* prog_name)
{
  printf("Usage %s: [options]\n", prog_name);
  printf("Options:\n");
  printf("  -h, --help       Show this help message\n");
  printf("  -c, --cpu        Show CPU info\n");                          
  printf("  -n, --network    Show info for specific network interface\n"); 
}

int main(int argc, char** argv)
{
  int opt;
  int opt_idx = 0;
  while ((opt = getopt_long(argc, argv, "hcgn", options, &opt_idx)) != -1) {
    switch (opt) {
      case 'h':
        print_usage(argv[0]);
        break;
      
        case 'c': {
          CPU* cpu = cpu_get_info();
          char* cpu_json = cpu_to_json(cpu);
          printf("%s", cpu_json);
          free(cpu);
          break;
      }

      default:
        break;
    }
  }   
  return 0;
}
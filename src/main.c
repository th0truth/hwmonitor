#include "base.h"
#include <getopt.h>
#include <cJSON.h>

#include "file.h"

#include "cpu.h"

static struct option options[] = {
  {"output",      required_argument,  NULL, 'o'},
  {"json",        no_argument,        NULL, 'j'},
  {"cpu",         no_argument,        NULL, 'c'},
  {"help",        no_argument,        NULL, 'h'},
  {NULL,          0,                  NULL,  0}
};

typedef struct
{
  char* output_file;
  bool show_cpu;
  bool use_json;
} Config;


void print_usage(const char* prog_name)
{
  printf("Usage %s: [options]\n", prog_name);
  printf("Options:\n");
  printf("  -h, --help       Show this help message\n");
  printf("  -c, --cpu        Show CPU info\n");                          
}

int main(int argc, char** argv)
{
  Config config = {0};
  char* output_file = NULL;

  int opt;
  int opt_idx = 0;
  while ((opt = getopt_long(argc, argv, "hcjo:", options, &opt_idx)) != -1) {
    switch (opt) {
      case 'j':
        config.use_json = true;
        break;
      case 'o':
        if (optarg) {
          config.output_file = strdup(optarg);
        }
        break;
      case 'c':
        config.show_cpu = true;
        break;
      case 'h':
        print_usage(argv[0]);
        return 0;
      default:
        break;
    }
  }
  
  // If NO hardware flags were set, but --json was, show EVERYTHING
  if (config.use_json && !config.show_cpu) {
    config.show_cpu = true;
  }

  // Collect data and build JSON
  if (config.use_json) {
    cJSON* json = cJSON_CreateObject();

    if (config.show_cpu) {
      CPU* cpu = cpu_get_info();
      cJSON_AddItemToObject(json, "cpu", cpu_to_json_obj(cpu));
      free_cpu(cpu);
    }

    char* json_str = cJSON_Print(json);
    if (config.output_file) {
      if (file_write_string(config.output_file, json_str)) {
        printf("Success: Hardware report saved to '%s'\n", config.output_file);
      }
    } else {
      printf("%s\n", json_str);
    }

    free(json_str);
    cJSON_Delete(json);
  }


  return 0;
}
#include "base.h"
#include <getopt.h>
#include <cJSON.h>

#include "file.h"
#include "cpu.h"
#include "ram.h"

static struct option options[] = {
  {"output",      required_argument,  NULL, 'o'},
  {"json",        no_argument,        NULL, 'j'},
  {"cpu",         no_argument,        NULL, 'c'},
  {"ram",         no_argument,        NULL, 'r'},
  {"help",        no_argument,        NULL, 'h'},
  {NULL,          0,                  NULL,  0}
};

typedef struct
{
  char* output_file;
  bool show_cpu;
  bool show_ram;
  bool use_json;
} Config;

void print_usage(const char* prog_name)
{
  printf("Usage %s: [options]\n", prog_name);
  printf("Options:\n");
  printf("  -h, --help       Show this help message\n");
  printf("  -c, --cpu        Show CPU info\n");
  printf("  -r, --ram        Show RAM info\n");
  printf("  -j, --json       Output in JSON format\n");
  printf("  -o, --output <f> Save output to file\n");
}

int main(int argc, char** argv)
{
  Config config = {0};
  int opt;
  int opt_idx = 0;

  while ((opt = getopt_long(argc, argv, "hcjro:", options, &opt_idx)) != -1) {
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
      case 'r':
        config.show_ram = true;
        break;
      case 'h':
        print_usage(argv[0]);
        return 0;
      default:
        break;
    }
  }
  
  // If NO hardware flags were set, but --json was, show EVERYTHING
  if (config.use_json && !config.show_cpu && !config.show_ram) {
    config.show_cpu = true;
    config.show_ram = true;
  }

  if (config.use_json) {
    cJSON* json = cJSON_CreateObject();

    if (config.show_cpu) {
      CPU* cpu = cpu_get_info();
      if (cpu) {
        cJSON_AddItemToObject(json, "cpu", cpu_to_json_obj(cpu));
        free_cpu(cpu);
      }
    }
    
    if (config.show_ram) {
      RAM* ram = ram_get_info();
      if (ram) {
        cJSON_AddItemToObject(json, "ram", ram_to_json_obj(ram));
        free_ram(ram);
      }
    }

    char* json_str = cJSON_Print(json);
    if (config.output_file) {
      if (file_write_string(config.output_file, json_str)) {
        printf("Success: Hardware report saved to '%s'\n", config.output_file);
      }
      free(config.output_file);
    } else {
      printf("%s\n", json_str);
    }

    free(json_str);
    cJSON_Delete(json);
  } else {
    if (config.show_cpu) {
      CPU* cpu = cpu_get_info();
      if (cpu) {
        printf("CPU: %s\n", STR_OR_UNK(cpu->model_name));
        free_cpu(cpu);
      }
    }
    if (config.show_ram) {
      RAM* ram = ram_get_info();
      if (ram) {
        printf("RAM Total: %lu kB\n", ram->total);
        free_ram(ram);
      }
    }
  }

  return 0;
}

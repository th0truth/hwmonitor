#include "base.h"
#include <getopt.h>

#include "file.h"
#include "display.h"

#include "cpu.h"
#include "ram.h"
#include "gpu.h"
#include "battery.h"

/**
 * Command-line options defined for getopt_long.
 */
static struct option options[] = {
  {"output",      required_argument,  NULL, 'o'},
  {"json",        no_argument,        NULL, 'j'},
  {"cpu",         no_argument,        NULL, 'c'},
  {"battery",     no_argument,        NULL, 'b'},
  {"ram",         no_argument,        NULL, 'r'},
  {"gpu",         no_argument,        NULL, 'g'},
  {"help",        no_argument,        NULL, 'h'},
  {NULL,          0,                  NULL,  0}
};

/**
 * Config structure to hold user preferences from flags.
 */
typedef struct
{
  char* output_file;
  bool use_json;
  bool show_battery;
  bool show_cpu;
  bool show_ram;
  bool show_gpu;
} Config;

/**
 * Prints usage and help documentation for the tool.
 * @param prog_name The executable name (argv[0]).
 */
void print_usage(const char* prog_name)
{
  printf("Usage %s: [options]\n", prog_name);
  printf("Options:\n");
  printf("  -h, --help       Show this help message\n");
  printf("  -c, --cpu        Show CPU information\n");
  printf("  -r, --ram        Show RAM information\n");
  printf("  -g, --gpu        Show GPU information\n");
  printf("  -j, --json       Output in formatted JSON\n");
  printf("  -o, --output <f> Save output to a JSON file\n");
}

/**
 * Main entry point of the application.
 * Manages configuration, hardware discovery, and final output.
 */
int main(int argc, char** argv)
{
  Config config = {0};
  int opt;
  int opt_idx = 0;

  // 1. Parse command-line arguments
  while ((opt = getopt_long(argc, argv, "hrbcgjro:", options, &opt_idx)) != -1) {
    switch (opt) {
      case 'j':
        config.use_json = true;
        break;
      case 'o':
        if (optarg) config.output_file = strdup(optarg);
        break;
      case 'b':
        config.show_battery = false;
        break;
      case 'c':
        config.show_cpu = true;
        break;
      case 'r':
        config.show_ram = true;
        break;
      case 'g':
        config.show_gpu = true;
        break;
      case 'h':
        print_usage(argv[0]);
        return 0;
      default:
        break;
    }
  }
  
  // 2. Default behavior: if --json is set without filters, show all hardware
  if (config.use_json && !config.show_cpu && !config.show_ram && !config.show_gpu && !config.show_battery) {
    config.show_cpu = true;
    config.show_ram = true;
    config.show_gpu = true;
    config.show_battery = true;
  }

  // 3. Output logic: JSON or Plain Text
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

    if (config.show_gpu) {
      int count = 0;
      GPU** gpus = gpu_get_all(&count);
      if (gpus) {
        cJSON* gpu_list = cJSON_CreateArray();
        for (int i = 0; i < count; i++) {
          cJSON_AddItemToArray(gpu_list, gpu_to_json_obj(gpus[i]));
        }
        cJSON_AddItemToObject(json, "gpus", gpu_list);
        free_gpus(gpus, count);
      }
    }

    if (config.show_battery) {
      BATTERY* battery = battery_get_info();
      if (battery) {
        cJSON_AddItemToObject(json, "battery", battery_to_json_obj(battery));
        free_battery(battery);
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
  } 
  else {
    // 4. Default: Basic Plain Text View
    if (config.show_cpu) {
      CPU* cpu = cpu_get_info();
      if (cpu) {
        display_cpu(cpu);
        free_cpu(cpu);
      }
    }
    if (config.show_ram) {
      RAM* ram = ram_get_info();
      if (ram) {
        printf("RAM Total: %" PRIu64 " kB\n", ram->total);
        free_ram(ram);
      }
    }
    if (config.show_gpu) {
      int count = 0;
      GPU** gpus = gpu_get_all(&count);
      if (gpus) {
        for (int i = 0; i < count; i++) {
          printf("GPU [%d]: %s (%s)\n", i, STR_OR_UNK(gpus[i]->model), STR_OR_UNK(gpus[i]->vendor));
        }
        free_gpus(gpus, count);
      }
    }
  }

  return 0;
}

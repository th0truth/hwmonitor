/**
 * @file hwmonitor.c
 * @brief Main entry point and orchestration logic for hwmonitor.
 */

#include "base.h"
#include <getopt.h>

#include "file.h"
#include "display.h"

#include "os.h"
#include "cpu.h"
#include "ram.h"
#include "gpu.h"
#include "battery.h"

#define SHORT_OPTS "hrbOcgjo:"

/**
 * @brief Command-line options defined for getopt_long.
 */
static struct option long_options[] = {
  {"output",      required_argument,  NULL, 'o'},
  {"json",        no_argument,        NULL, 'j'},
  {"os",          no_argument,        NULL, 'O'},
  {"cpu",         no_argument,        NULL, 'c'},
  {"battery",     no_argument,        NULL, 'b'},
  {"ram",         no_argument,        NULL, 'r'},
  {"gpu",         no_argument,        NULL, 'g'},
  {"help",        no_argument,        NULL, 'h'},
  {NULL,          0,                  NULL,  0}
};

/**
 * @brief Config structure to hold user preferences from flags.
 */
typedef struct {
  char* output_file;
  bool use_json;
  bool show_os;
  bool show_cpu;
  bool show_ram;
  bool show_gpu;
  bool show_battery;
} Config;

/**
 * @brief Centralized struct to hold all fetched hardware data.
 */
typedef struct {
  OS* os;
  CPU* cpu;
  RAM* ram;
  GPU** gpus;
  int gpu_count;
  BATTERY* battery;
} SystemHardware;

/**
 * Prints usage and help documentation for the tool.
 * @param prog_name The executable name (argv[0]).
 */
void print_usage(const char* prog_name)
{
  printf("Usage: %s [options]\n", prog_name);
  printf("Options:\n");
  printf("  -h, --help       Show this help message\n");
  printf("  -O, --os         Show OS information\n");
  printf("  -c, --cpu        Show CPU information\n");
  printf("  -r, --ram        Show RAM information\n");
  printf("  -g, --gpu        Show GPU information\n");
  printf("  -b, --battery    Show Battery information\n");
  printf("  -j, --json       Output in formatted JSON\n");
  printf("  -o, --output <f> Save output to a JSON file\n");
}

/**
 * @brief Parses command-line arguments and populates the Config struct.
 */
void parse_arguments(int argc, char** argv, Config* config)
{
  int opt;
  int opt_idx = 0;

  while ((opt = getopt_long(argc, argv, SHORT_OPTS, long_options, &opt_idx)) != -1) {
    switch (opt) {
      case 'j':
        config->use_json = true;
        break;
      case 'o':
        if (optarg) config->output_file = strdup(optarg);
        config->use_json = true; // Implicitly enable JSON output when saving to file
        break;
      case 'c':
        config->show_cpu = true;
        break;
      case 'O':
        config->show_os = true;
        break;
      case 'r':
        config->show_ram = true;
        break;
      case 'g':
        config->show_gpu = true;
        break;
      case 'b':
        config->show_battery = true;
        break;
      case 'h':
        print_usage(argv[0]);
        exit(0);
      default:
        print_usage(argv[0]);
        exit(1);
    }
  }

  // Default behavior: if no specific hardware filters are set, show all
  if (!config->show_os && !config->show_cpu && !config->show_ram && !config->show_gpu && !config->show_battery) {
    config->show_os = true;
    config->show_cpu = true;
    config->show_ram = true;
    config->show_gpu = true;
    config->show_battery = true;
  }
}

/**
 * @brief Fetches required hardware data based on configuration.
 */
void fetch_hardware(const Config* config, SystemHardware* hw)
{
  if (config->show_os)
    hw->os = os_get_info();
  if (config->show_cpu) 
    hw->cpu = cpu_get_info();
  if (config->show_ram) 
    hw->ram = ram_get_info();
  if (config->show_gpu)
    hw->gpus = gpu_get_all(&hw->gpu_count);
  if (config->show_battery)
    hw->battery = battery_get_info();
}

/**
 * @brief Frees all allocated memory within the SystemHardware struct.
 */
void free_hardware(SystemHardware* hw)
{
  if (hw->os)
    free(hw->os);
  if (hw->cpu)
    free_cpu(hw->cpu);
  if (hw->ram)
    free_ram(hw->ram);
  if (hw->gpus)
    free_gpus(hw->gpus, hw->gpu_count);
  if (hw->battery)
    free_battery(hw->battery);
}

/**
 * @brief Generates and prints (or saves) the JSON output.
 */
void output_json(const Config* config, const SystemHardware* hw)
{
  cJSON* json = cJSON_CreateObject();

  if (hw->cpu)
    cJSON_AddItemToObject(json, "cpu", cpu_to_json_obj(hw->cpu));
  
  if (hw->ram)
    cJSON_AddItemToObject(json, "ram", ram_to_json_obj(hw->ram));
  
  if (hw->gpus && hw->gpu_count > 0) {
    cJSON* gpu_list = cJSON_CreateArray();
    for (int i = 0; i < hw->gpu_count; i++) {
      cJSON_AddItemToArray(gpu_list, gpu_to_json_obj(hw->gpus[i]));
    }
    cJSON_AddItemToObject(json, "gpus", gpu_list);
  }

  if (hw->battery)
    cJSON_AddItemToObject(json, "battery", battery_to_json_obj(hw->battery));

  char* json_str = cJSON_Print(json);
  if (config->output_file) {
    if (file_write_string(config->output_file, json_str)) {
      printf("Success: Hardware report saved to '%s'\n", config->output_file);
    }
  } else {
    printf("%s\n", json_str);
  }

  free(json_str);
  cJSON_Delete(json);
}

/**
 * @brief Renders the hardware data in a formatted plain-text view.
 */
void output_plaintext(const SystemHardware* hw)
{
  if (hw->os)
    display_os(hw->os);
  if (hw->cpu)
    display_cpu(hw->cpu);
  if (hw->ram)
    display_ram(hw->ram);
  if (hw->gpus && hw->gpu_count > 0)
    display_gpus(hw->gpus, hw->gpu_count);
  if (hw->battery)
    display_battery(hw->battery);
}

/**
 * Main entry point of the application.
 * Manages configuration, hardware discovery, and final output.
 */
int main(int argc, char** argv)
{
  Config config = {0};
  SystemHardware hw = {0};

  parse_arguments(argc, argv, &config);
  fetch_hardware(&config, &hw);

  if (config.use_json) {
    output_json(&config, &hw);
  } else {
    output_plaintext(&hw);
  }

  free_hardware(&hw);
  
  if (config.output_file) {
    free(config.output_file);
  }

  return 0;
}

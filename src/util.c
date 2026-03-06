/**
 * @file util.c
 * @brief General utility functions, hardware orchestration, and CLI parsing.
 */

#include "base.h"
#include "util.h"
#include "display.h"
#include "file.h"

#include <getopt.h>
#include <sys/sysinfo.h>

#define SHORT_OPTS "aA:bcghjmno:rsOw"

/**
 * @brief Command-line options defined for getopt_long (Sorted Alphabetically).
 */
static struct option long_options[] = {
  {"ai",          required_argument,  NULL, 'A'},
  {"all",         no_argument,        NULL, 'a'},
  {"battery",     no_argument,        NULL, 'b'},
  {"cpu",         no_argument,        NULL, 'c'},
  {"gpu",         no_argument,        NULL, 'g'},
  {"help",        no_argument,        NULL, 'h'},
  {"json",        no_argument,        NULL, 'j'},
  {"mainboard",   no_argument,        NULL, 'm'},
  {"network",     no_argument,        NULL, 'n'},
  {"os",          no_argument,        NULL, 'O'},
  {"output",      required_argument,  NULL, 'o'},
  {"ram",         no_argument,        NULL, 'r'},
  {"storage",     no_argument,        NULL, 's'},
  {"watch",       no_argument,        NULL, 'w'},
  {NULL,          0,                  NULL,  0}
};

/**
 * @brief Fetches required hardware data based on configuration.
 */
void fetch_hardware(const Config* config, SystemHardware* hw)
{
  if (config->show_battery)
    hw->battery = battery_get_info();
  if (config->show_cpu) 
    hw->cpu = cpu_get_info();
  if (config->show_gpu)
    hw->gpus = gpu_get_all(&hw->gpu_count);
  if (config->show_mainboard)
    hw->mainboard = mainboard_get_info();
  if (config->show_network)
    hw->networks = network_get_all(&hw->network_count);
  if (config->show_os)
    hw->os = os_get_info();
  if (config->show_ram) 
    hw->ram = ram_get_info();
  if (config->show_storage)
    hw->storages = storage_get_all(&hw->storage_count);
}

/**
 * @brief Formats a byte size into a human-readable string.
 */
void format_size(char* target_format, uint64_t bytes, char* buffer, size_t buf_size)
{
  const char* suffixes[] = {"EiB", "PiB", "TiB", "GiB", "MiB", "KiB", "B"};
  uint64_t multiplier = 1ULL << 60; 
  int target_idx = 0;
  int num_suffixes = sizeof(suffixes) / sizeof(suffixes[0]);

  for (; target_idx < num_suffixes; target_idx++, multiplier >>= 10) {
    if (strcmp(suffixes[target_idx], target_format) == 0)
      break;
  }

  if (target_idx == num_suffixes) {
    target_idx = num_suffixes - 1; 
    multiplier = 1;
  }

  double final_value = (double)bytes / (double)multiplier;
  snprintf(buffer, buf_size, "%.2f %s", final_value, suffixes[target_idx]);
}

/**
 * @brief Frees all allocated memory within the SystemHardware struct.
 */
void free_hardware(SystemHardware* hw)
{
  if (hw->battery)
    free_battery(hw->battery);
  if (hw->cpu)
    free_cpu(hw->cpu);
  if (hw->gpus)
    free_gpus(hw->gpus, hw->gpu_count);
  if (hw->mainboard)
    free_mainboard(hw->mainboard);
  if (hw->networks)
    free_networks(hw->networks, hw->network_count);
  if (hw->os)
    free(hw->os);
  if (hw->ram)
    free_ram(hw->ram);
  if (hw->storages)
    free_storages(hw->storages, hw->storage_count);
}

/**
 * @brief Generates and prints (or saves) the JSON output.
 */
void output_json(const Config* config, const SystemHardware* hw)
{
  cJSON* json = cJSON_CreateObject();
  
  if (hw->battery)
    cJSON_AddItemToObject(json, "battery", battery_to_json_obj(hw->battery));
  if (hw->cpu)
    cJSON_AddItemToObject(json, "cpu", cpu_to_json_obj(hw->cpu));
  if (hw->gpus && hw->gpu_count > 0) {
    cJSON* gpu_list = cJSON_CreateArray();
    for (int i = 0; i < hw->gpu_count; i++) {
      cJSON_AddItemToArray(gpu_list, gpu_to_json_obj(hw->gpus[i]));
    }
    cJSON_AddItemToObject(json, "gpus", gpu_list);
  }
  if (hw->mainboard)
    cJSON_AddItemToObject(json, "mainboard", mainboard_to_json_obj(hw->mainboard));
  if (hw->networks && hw->network_count > 0) {
    cJSON* network_list = cJSON_CreateArray();
    for (int i = 0; i < hw->network_count; i++) {
      cJSON_AddItemToArray(network_list, network_to_json_obj(hw->networks[i]));
    }
    cJSON_AddItemToObject(json, "networks", network_list);
  }
  if (hw->os)
    cJSON_AddItemToObject(json, "os", os_to_json_obj(hw->os));
  if (hw->ram)
    cJSON_AddItemToObject(json, "ram", ram_to_json_obj(hw->ram));
  if (hw->storages && hw->storage_count > 0) {
    cJSON* storage_list = cJSON_CreateArray();
    for (int i = 0; i < hw->storage_count; i++) {
      cJSON_AddItemToArray(storage_list, storage_to_json_obj(hw->storages[i]));
    }
    cJSON_AddItemToObject(json, "storages", storage_list);
  }

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
  if (hw->battery)
    display_battery(hw->battery);
  if (hw->cpu)
    display_cpu(hw->cpu);
  if (hw->gpus && hw->gpu_count > 0)
    display_gpus(hw->gpus, hw->gpu_count);
  if (hw->mainboard)
    display_mainboard(hw->mainboard);
  if (hw->networks && hw->network_count > 0)
    display_networks(hw->networks, hw->network_count);
  if (hw->os)
    display_os(hw->os);
  if (hw->ram)
    display_ram(hw->ram);
  if (hw->storages && hw->storage_count > 0)
    display_storages(hw->storages, hw->storage_count);
}

/**
 * @brief Parses command-line arguments (Sorted Alphabetically).
 */
void parse_arguments(int argc, char** argv, Config* config)
{
  int opt;
  int opt_idx = 0;
  while ((opt = getopt_long(argc, argv, SHORT_OPTS, long_options, &opt_idx)) != -1) {
    switch (opt) {
      case 'a':
        config->show_battery = true;
        config->show_cpu = true;
        config->show_gpu = true;
        config->show_mainboard = true;
        config->show_network = true;
        config->show_os = true;
        config->show_ram = true;
        config->show_storage = true;
        break;
      case 'A':
        config->use_ai = true;
        if (optarg) config->ai_prompt = strdup(optarg);
        break;
      case 'b':
        config->show_battery = true;
        break;
      case 'c':
        config->show_cpu = true;
        break;
      case 'g':
        config->show_gpu = true;
        break;
      case 'h':
        print_usage(argv[0]);
        exit(0);
      case 'j':
        config->use_json = true;
        break;
      case 'm':
        config->show_mainboard = true;
        break;
      case 'n':
        config->show_network = true;
        break;
      case 'O':
        config->show_os = true;
        break;
      case 'o':
        if (optarg) config->output_file = strdup(optarg);
        config->use_json = true;
        break;
      case 'r':
        config->show_ram = true;
        break;
      case 's':
        config->show_storage = true;
        break;
      case 'w':
        config->watch_mode = true;
        break;
      default:
        print_usage(argv[0]);
        exit(1);
    }
  }

  // Default: show all if no filters set
  if (!config->show_battery && !config->show_cpu && !config->show_gpu && 
      !config->show_mainboard && !config->show_network && !config->show_os && 
      !config->show_ram && !config->show_storage && !config->use_ai) {
    config->show_battery = true;
    config->show_cpu = true;
    config->show_gpu = true;
    config->show_mainboard = true;
    config->show_network = true;
    config->show_os = true;
    config->show_ram = true;
    config->show_storage = true;
  }
}

/**
 * @brief Prints usage and help documentation (Sorted Alphabetically).
 */
void print_usage(const char* prog_name)
{
  printf("Usage: %s [options]\n", prog_name);
  printf("Options:\n");
  printf("  -A, --ai <text>  Analyze hardware using Groq AI with a prompt\n");
  printf("  -a, --all        Show ALL hardware information\n");
  printf("  -b, --battery    Show Battery information\n");
  printf("  -c, --cpu        Show CPU information\n");
  printf("  -g, --gpu        Show GPU information\n");
  printf("  -h, --help       Show this help message\n");
  printf("  -j, --json       Output in formatted JSON\n");
  printf("  -m, --mainboard  Show Mainboard/System information\n");
  printf("  -n, --network    Show Network interfaces information\n");
  printf("  -O, --os         Show OS information\n");
  printf("  -o, --output <f> Save output to a JSON file\n");
  printf("  -r, --ram        Show RAM information\n");
  printf("  -s, --storage    Show Storage/Disk information\n");
  printf("  -w, --watch      Live refresh hardware data every second\n");
}

/**
 * @brief Returns the system load averages (1, 5, 15 min).
 */
void util_get_loadavg(double load[3])
{
  if (getloadavg(load, 3) != 3) {
    load[0] = load[1] = load[2] = 0.0;
  }
}

/**
 * @brief Returns the current system uptime in seconds.
 */
double util_get_uptime(void)
{
  FILE* fp = fopen("/proc/uptime", "r");
  if (!fp) return 0.0;
  double uptime;
  if (fscanf(fp, "%lf", &uptime) != 1) uptime = 0.0;
  fclose(fp);
  return uptime;
}

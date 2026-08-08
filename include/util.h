/**
 * \file util.h
 * \brief Header file for utility orchestration, formatting, and CLI parsing.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "base.h"
#include "os.h"
#include "cpu.h"
#include "ram.h"
#include "gpu.h"
#include "battery.h"
#include "mainboard.h"
#include "storage.h"
#include "network.h"

/**
 * \struct Config
 * \brief Config structure to hold user preferences from flags.
 */
typedef struct {
  char *ai_prompt;
  char *output_file;
  bool show_battery;
  bool show_cpu;
  bool show_gpu;
  bool show_mainboard;
  bool show_network;
  bool show_os;
  bool show_ram;
  bool show_storage;
  bool use_ai;
  bool use_json;
  bool watch_mode;
} Config;

/**
 * \struct SystemHardware
 * \brief Centralized struct to hold all fetched hardware data.
 */
typedef struct {
  BATTERY *battery;
  CPU *cpu;
  GPU **gpus;
  int gpu_count;
  MAINBOARD *mainboard;
  Network **networks;
  int network_count;
  OS *os;
  RAM *ram;
  STORAGE **storages;
  int storage_count;
} SystemHardware;

/**
 * \brief Fetches required hardware data based on configuration.
 * \param[in] config Pointer to the Config structure.
 * \param[out] hw Pointer to the SystemHardware structure to populate.
 */
void fetch_hardware(const Config *config, SystemHardware *hw);

/**
 * \brief Frees all allocated memory within the SystemHardware struct.
 * \param[in,out] hw Pointer to the SystemHardware structure.
 */
void free_hardware(SystemHardware *hw);

/**
 * \brief Formats a byte size into a human-readable string.
 * \param[in] target_format Format unit (e.g. "GiB", "MiB").
 * \param[in] bytes Number of bytes.
 * \param[out] buffer Output character buffer.
 * \param[in] buf_size Size of the output buffer.
 */
void format_size(char *target_format, uint64_t bytes, char *buffer, size_t buf_size);

/**
 * \brief Generates and prints (or saves) the JSON output.
 * \param[in] config Pointer to the Config structure.
 * \param[in] hw Pointer to the SystemHardware structure.
 */
void output_json(const Config *config, const SystemHardware *hw);

/**
 * \brief Renders the hardware data in a formatted plain-text view.
 * \param[in] hw Pointer to the SystemHardware structure.
 */
void output_plaintext(const SystemHardware *hw);

/**
 * \brief Parses command-line arguments and populates the Config struct.
 * \param[in] argc Argument count.
 * \param[in] argv Argument array.
 * \param[out] config Pointer to the Config structure.
 */
void parse_arguments(int argc, char **argv, Config *config);

/**
 * \brief Prints usage and help documentation for the tool.
 * \param[in] prog_name Name of the program binary.
 */
void print_usage(const char *prog_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

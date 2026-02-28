/**
 * @file util.c
 * @brief General utility functions.
 */

#include "base.h"

/**
 * @brief Formats a byte size into a human-readable string (e.g., KB, MB, GB).
 * @param target_format The target format to output (e.g., "MiB", "GiB").
 * @param bytes The size in bytes to format.
 * @param buffer Output buffer to write the formatted string.
 * @param buf_size Maximum size of the output buffer.
 */
void format_size(char* target_format, uint64_t bytes, char* buffer, size_t buf_size)
{
  const char* suffixes[] = {"EiB", "PiB", "TiB", "GiB", "MiB", "KiB", "B"};
  
  // Start at EiB (2^60)
  uint64_t multiplier = 1ULL << 60; 
  
  int target_idx = 0;
  int num_suffixes = sizeof(suffixes) / sizeof(suffixes[0]);

  // Find the matching suffix and its corresponding multiplier
  for (; target_idx < num_suffixes; target_idx++, multiplier >>= 10) {
    if (strcmp(suffixes[target_idx], target_format) == 0)
      break;
  }

  // If the string wasn't found, default to Bytes
  if (target_idx == num_suffixes) {
    target_idx = num_suffixes - 1; 
    multiplier = 1;
  }

  // Calculate the final value and format it into the buffer
  double final_value = (double)bytes / (double)multiplier;
  snprintf(buffer, buf_size, "%.2f %s", final_value, suffixes[target_idx]);
}

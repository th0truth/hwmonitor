/**
 * @file util.h
 * @brief Header file for util module.
 */

#pragma once

#include "base.h"

/**
 * @brief Formats a byte size into a human-readable string (e.g., KB, MB, GB).
 * @param format Format string (unused directly, preserved for compatibility or refactoring).
 * @param size Size in bytes.
 * @param buffer Output buffer to write the formatted string.
 * @param buf_size Maximum size of the output buffer.
 */
void format_size(char* format, uint64_t size, char* buffer, size_t buf_size);
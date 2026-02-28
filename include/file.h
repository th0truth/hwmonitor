/**
 * @file file.h
 * @brief Header file for file module.
 */

#pragma once

/**
 * @brief Reads a file into a dynamically allocated string, stripping specified characters.
 * @param filename Path to the file to read.
 * @param exclude Characters to strip out from the file content.
 * @param verbose If true, prints warning messages on failure.
 * @return A null-terminated dynamically allocated string, or NULL on error.
 */
char* file_read_stripped(const char* filename, const char* exclude, bool verbose);
/**
 * @brief Writes a string to a file.
 * @param filename Path to the file to write.
 * @param data String data to write.
 * @return true on success, false on failure.
 */
bool file_write_string(const char* filename, const char* data);
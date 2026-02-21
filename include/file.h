#pragma once

char* file_read_stripped(const char* filename, const char* exclude, bool verbose);
bool file_write_string(const char* filename, const char* data);
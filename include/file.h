#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdbool.h>

char *file_read_stripped(const char *filename, const char *exclude, bool verbose);
bool file_write_string(const char *filename, const char *data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

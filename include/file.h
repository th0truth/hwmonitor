#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdbool.h>

char *file_read_stripped(const char *filename, const char *exclude, bool verbose);
bool file_write_string(const char *filename, const char *data);

typedef void *(*sysfs_parse_fn)(const char *entry_name);
void **sysfs_enumerate(const char *dir_path, sysfs_parse_fn parse_fn, int max_items, int *out_count); 
char *sysfs_read_attr_fmt(const char *exclude, const char *fmt, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "base.h"

char *str_find_value(const char *source, const char *search, const char *delim);
double str_parse_value(const char *source, const char *search, const char *delim);

#ifdef __cplusplus
}
#endif /* __cplusplus */

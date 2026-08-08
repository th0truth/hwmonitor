/**
 * \file io.h
 * \brief Header file for string parsing utilities.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "base.h"

/**
 * \brief Searches a source string for a key and returns the associated value.
 * \param[in] source The source string to search.
 * \param[in] search The key string to find.
 * \param[in] delim Delimiter characters to find the end of the value.
 * \return Dynamically allocated string (caller must free), or NULL if not found.
 */
char *str_find_value(const char *source, const char *search, const char *delim);

/**
 * \brief Searches a source string for a key and returns the value as a double.
 * \param[in] source The source string to search.
 * \param[in] search The key string to find.
 * \param[in] delim Delimiter characters to find the end of the value.
 * \return Value as a double, or 0.0 if not found.
 */
double str_parse_value(const char *source, const char *search, const char *delim);

#ifdef __cplusplus
}
#endif /* __cplusplus */

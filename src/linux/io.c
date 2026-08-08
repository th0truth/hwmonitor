#include "io.h"

char *
str_find_value(const char *source, const char *search, const char *delim)
{
    if (source == NULL || search == NULL || delim == NULL) {
        return NULL;
    }

    size_t search_len = strlen(search);
    const char *current = source;

    while ((current = strstr(current, search)) != NULL) {
        bool valid_start = (current == source || *(current - 1) == '\n');

        if (valid_start) {
            const char *start = current + search_len;

            /* Skip leading separators and whitespaces to reach the start of the value */
            while (*start != '\0' && (isspace((unsigned char)*start) || *start == ':' || *start == '=')) {
                ++start;
            }

            /* Find the end of the value based on the provided delimiters */
            const char *end = start;
            while (*end != '\0' && strchr(delim, *end) == NULL) {
                ++end;
            }

            /* Backtrack to trim any trailing whitespace */
            while (end > start && isspace((unsigned char)*(end - 1))) {
                --end;
            }

            uint64_t len = end - start;
            if (len == 0) {
                return NULL;
            }

            /* Allocate memory and copy the resulting value */
            char *value = malloc(len + 1);
            if (value == NULL) {
                return NULL;
            }

            memcpy(value, start, len);
            value[len] = '\0';

            return value;
        }
        current += 1;
    }

    return NULL;
}

double
str_parse_value(const char *source, const char *search, const char *delim)
{
    char *value = str_find_value(source, search, delim);
    if (value == NULL) {
        return 0.0;
    }

    double result = atof(value);
    free(value);

    return result;
}

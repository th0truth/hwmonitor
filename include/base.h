#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <cJSON.h>

#define BUFFER_SIZE 256

#define STR_OR_UNK(s) ((s) ? (s) : "<unknown>")

#define DEFINE_FREE_ARRAY(func_name, item_type, free_item_fn) \
    void func_name(item_type **items, int count)              \
    {                                                         \
        if (items == NULL) {                                  \
            return;                                           \
        }                                                     \
                                                              \
        for (int i = 0; i < count; i++) {                     \
            if (items[i] != NULL) {                           \
                free_item_fn(items[i]);                       \
            }                                                 \
        }                                                     \
                                                              \
        free(items);                                          \
    }                                                           \
    
#ifdef __cplusplus
}
#endif /* __cplusplus */

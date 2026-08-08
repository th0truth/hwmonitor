#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <cJSON.h>

typedef struct {
    char *name;
    char *version_id;
    char *build_id;
    char *release_type;
    char *id;
    char *de;
    char *de_id;
} OS;

OS *os_get_info(void);
void free_os(OS *os);
cJSON *os_to_json_obj(const OS *os);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdbool.h>
#include <stddef.h>
#include <curl/curl.h>

typedef struct {
    char *data;
    size_t size;
} HttpResponse;

bool http_post(const char *url, struct curl_slist *headers, const char *payload, HttpResponse *out_response);
void http_free_response(HttpResponse *response);

#ifdef __cplusplus
}
#endif /* __cplusplus */

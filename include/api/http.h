/**
 * \file http.h
 * \brief Generic HTTP networking subsystem using libcurl.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdbool.h>
#include <stddef.h>
#include <curl/curl.h>

/**
 * \struct HttpResponse
 * \brief Buffer to store the dynamically allocated raw string response from an HTTP request.
 */
typedef struct {
    char *data;   /**< Null-terminated string containing the HTTP response body */
    size_t size;  /**< Size of the response data in bytes (excluding null terminator) */
} HttpResponse;

/**
 * \brief Performs a blocking HTTP POST request.
 * \param[in] url The target endpoint URL.
 * \param[in] headers A linked list of HTTP headers (e.g., Authorization, Content-Type).
 * \param[in] payload The raw string payload to send in the body of the request.
 * \param[out] out_response Pointer to an HttpResponse struct to hold the result.
 *                          The caller MUST call http_free_response() on success to avoid leaks.
 * \return true if the network request succeeded (CURLE_OK), false otherwise.
 */
bool http_post(const char *url, struct curl_slist *headers, const char *payload, HttpResponse *out_response);

/**
 * \brief Safely frees the dynamically allocated memory inside an HttpResponse.
 * \param[in,out] response Pointer to the HttpResponse to clean up.
 */
void http_free_response(HttpResponse *response);

#ifdef __cplusplus
}
#endif /* __cplusplus */

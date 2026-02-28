#include "api/http.h"
#include <stdlib.h>
#include <string.h>

static size_t http_write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
  size_t realsize = size * nmemb;
  HttpResponse* mem = (HttpResponse*)userp;
  
  // Reallocate buffer to hold existing data + new chunk + null terminator
  char* ptr = realloc(mem->data, mem->size + realsize + 1);
  if (!ptr)
    return 0;
  
  mem->data = ptr;
  memcpy(&(mem->data[mem->size]), contents, realsize);
  
  mem->size += realsize;
  mem->data[mem->size] = '\0';
  
  return realsize;
}

bool http_post(const char* url, struct curl_slist* headers, const char* payload, HttpResponse* out_response)
{
  if (!url || !payload || !out_response)
    return false;
  
  CURL* curl;
  CURLcode res;
  
  // Initialize the response structure with a 1-byte null-terminated string
  out_response->data = malloc(1);
  out_response->size = 0;
  
  if (!out_response->data)
    return false;
    
  out_response->data[0] = '\0';
  
  // Initialize a curl easy handle
  curl = curl_easy_init();
  if (!curl) {
    http_free_response(out_response);
    return false;
  }
  
  // Configure the request targets
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
  
  // Bind the write callback to our struct
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)out_response);
  
  // In a CLI tool, we usually want to follow redirects if they happen
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  // Execute the blocking network request
  res = curl_easy_perform(curl);

  // Cleanup the easy handle
  curl_easy_cleanup(curl);
  
  // If the request failed, free the partial buffer and return false
  if (res != CURLE_OK) {
    http_free_response(out_response);
    return false;
  }
  return true;
}

void http_free_response(HttpResponse* response)
{
  if (response && response->data) {
    free(response->data);
    response->data = NULL;
    response->size = 0;
  }
}
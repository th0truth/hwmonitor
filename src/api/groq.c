#include "base.h"
#include "api/http.h"
#include "api/groq.h"
#include "cJSON.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define GROQ_API_URL "https://api.groq.com/openai/v1/chat/completions"

/**
 * Builds the specific JSON payload required by Groq's chat completion endpoint.
 * @param hardware_json The raw hardware context.
 * @param user_prompt The command passed by the user.
 * @return Dynamically allocated JSON string, or NULL on failure.
 */
static char* build_groq_payload(const char* hardware_json, const char* user_prompt)
{
  cJSON* root = cJSON_CreateObject();
  if (!root)
    return NULL;
    
  // Determine model (allow user override for power users)
  const char* model = getenv("GROQ_MODEL");
  if (!model)
    model = GROQ_DEFAULT_MODEL;
  cJSON_AddStringToObject(root, "model", model);
  cJSON* messages = cJSON_CreateArray();
  if (!messages) {
    cJSON_Delete(root);
    return NULL;
  }
  cJSON_AddItemToObject(root, "messages", messages);
  // System Role: Context for the AI
  cJSON* sys_msg = cJSON_CreateObject();
  cJSON_AddStringToObject(sys_msg, "role", "system");
  cJSON_AddStringToObject(sys_msg, "content", 
    "You are an expert Linux system administrator. Analyze the provided JSON hardware telemetry "
    "and answer the user's prompt. "
    "Rules: 1) Be exceptionally concise. 2) DO NOT use markdown code blocks (like ```bash). "
    "3) Format your answer as a plain conversational paragraph or simple bullet points without markdown formatting. "
    "4) Do not repeat the raw hardware stats back to the user unless absolutely necessary.");
  cJSON_AddItemToArray(messages, sys_msg);
  // User Role: The data and the question
  size_t combined_len = strlen(hardware_json) + strlen(user_prompt) + 128;
  char* combined_content = malloc(combined_len);
  
  if (combined_content) {
    snprintf(combined_content, combined_len, 
             "Hardware Telemetry:\n%s\n\nUser Question:\n%s", 
             hardware_json, user_prompt);
    cJSON* usr_msg = cJSON_CreateObject();
    cJSON_AddStringToObject(usr_msg, "role", "user");
    cJSON_AddStringToObject(usr_msg, "content", combined_content);
    cJSON_AddItemToArray(messages, usr_msg);
    free(combined_content);
  }
  char* payload_str = cJSON_PrintUnformatted(root);
  cJSON_Delete(root);
  return payload_str;
}

bool groq_analyze_hardware(const char* hardware_json, const char* user_prompt)
{
  if (!hardware_json || !user_prompt)
    return false;

  const char* api_key = getenv("GROQ_API_KEY");
  if (!api_key || strlen(api_key) == 0) {
    fprintf(stderr, " \033[1;31m[ERROR]\033[0m Missing GROQ_API_KEY environment variable. Cannot query AI.\n");
    return false;
  }

  char* payload = build_groq_payload(hardware_json, user_prompt);
  if (!payload) {
    fprintf(stderr, " \033[1;31m[ERROR]\033[0m Failed to build Groq JSON payload.\n");
    return false;
  }

  // Set up HTTP headers
  struct curl_slist* headers = NULL;
  char auth_header[256];
  snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
  
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, auth_header);

  HttpResponse response = {0};
  bool success = false;

  // Print a loading indicator matching your theme (optional, can be disabled if too verbose)
  // printf(" \033[1;36m[...]\033[0m Querying AI Engine...\n\n");

  if (http_post(GROQ_API_URL, headers, payload, &response)) {
    cJSON* res_json = cJSON_Parse(response.data);
    
    if (res_json) {
      cJSON* choices = cJSON_GetObjectItem(res_json, "choices");
      if (cJSON_IsArray(choices) && cJSON_GetArraySize(choices) > 0) {
        cJSON* first_choice = cJSON_GetArrayItem(choices, 0);
        cJSON* message = cJSON_GetObjectItem(first_choice, "message");
        cJSON* content = cJSON_GetObjectItem(message, "content");
        
        if (cJSON_IsString(content) && content->valuestring) {
          // Print matching the aesthetic of src/display.c
          printf("\n\033[1;36m╭─ AI Hardware Analysis (Groq) \033[0m\n");
          
          // Print line by line with the left border
          char* text = content->valuestring;
          char* line = strtok(text, "\n");
          while (line != NULL) {
            printf("\033[1;36m|\033[0m \033[1;37m%s\033[0m\n", line);
            line = strtok(NULL, "\n");
          }
          
          printf("\033[1;36m╰─\033[0m\n");
          success = true;
        }
      } else {
        // Handle API errors gracefully
        cJSON* err = cJSON_GetObjectItem(res_json, "error");
        if (err) {
          cJSON* err_msg = cJSON_GetObjectItem(err, "message");
          if (cJSON_IsString(err_msg)) {
             fprintf(stderr, " \033[1;31m[API ERROR]\033[0m %s\n", err_msg->valuestring);
          }
        }
      }
      cJSON_Delete(res_json);
    } else {
      fprintf(stderr, " \033[1;31m[ERROR]\033[0m Failed to parse Groq API response.\n");
    }
  } else {
     fprintf(stderr, " \033[1;31m[ERROR]\033[0m Network request to Groq API failed.\n");
  }

  // Deep Cleanup (Zero Leaks)
  free(payload);
  curl_slist_free_all(headers);
  http_free_response(&response);

  return success;
}

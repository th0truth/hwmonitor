#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "base.h"

#define GROQ_DEFAULT_MODEL "openai/gpt-oss-20b"

bool groq_analyze_hardware(const char *hardware_json, const char *user_prompt);

#ifdef __cplusplus
}
#endif /* __cplusplus */

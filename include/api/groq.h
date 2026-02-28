#pragma once

#include "base.h"

/**
 * @brief Default model optimized for fast CLI usage.
 */
#define GROQ_DEFAULT_MODEL "llama-3.1-8b-instant"
/**
 * @brief Queries Groq to analyze the provided hardware telemetry.
 * 
 * @param hardware_json The JSON representation of the system/hardware.
 * @param user_prompt The user's specific question or command.
 * @return true on successful API request and parsing, false otherwise.
 */
bool groq_analyze_hardware(const char* hardware_json, const char* user_prompt);

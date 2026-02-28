/**
 * @file theme.h
 * @brief Header file for theme module.
 */

#pragma once

/**
 * @enum ThemeColor
 * @brief Available theme color categories.
 */
typedef enum {
  COLOR_PRIMARY,
  COLOR_LABEL,
  COLOR_VALUE,
  COLOR_SUCCESS,
  COLOR_WARNING,
  COLOR_DANGER,
  COLOR_RESET
} ThemeColor;

/**
 * @struct Theme
 * @brief Holds ANSI escape codes for styling console output.
 */
typedef struct {
  const char* primary;
  const char* label;
  const char* value;
  const char* success;
  const char* warning;
  const char* danger;
} Theme;

void theme_init(bool force_no_color);
char* theme_get_reset(void);
const char* theme_get_color(ThemeColor color);

void print_header(const char* text);
void print_field(const char* label, const char* format, ...);
void print_footer(void);
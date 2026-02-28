/**
 * @file theme.c
 * @brief Terminal theme and color formatting support.
 */

#include "base.h"
#include "theme.h"

#include <stdarg.h>
#include <unistd.h>

static bool use_colors = true;
static const char* RESET_CODE = "\033[0m";

static const Theme DarkTheme =
{
  .primary  = "\033[1;36m", // Cyan
  .label    = "\033[0;37m", // Light Gray
  .value    = "\033[1;37m", // White
  .success  = "\033[0;32m", // Green
  .warning  = "\033[0;33m", // Yellow
  .danger   = "\033[0;31m", // Red
};

/**
 * @brief Initializes the theme system.
 * Disables colors if force_no_color is true or stdout is not a terminal.
 * @param force_no_color User override to explicitly disable color output.
 */
void theme_init(bool force_no_color)
{
  // Crucial Low-Level Check: Is stdout a terminal or a file/pipe?
  if (force_no_color || !isatty(STDOUT_FILENO)) {
    use_colors = false;
  }
}

/**
 * @brief Retrieves the ANSI escape code for a specific ThemeColor.
 * @param color The theme color category to fetch.
 * @return String containing the ANSI code, or empty string if colors are disabled.
 */
const char* theme_get_color(ThemeColor color)
{
  if (!use_colors)
    return "";

  switch (color)
  {
    case COLOR_PRIMARY: return DarkTheme.primary;
    case COLOR_LABEL:   return DarkTheme.label;
    case COLOR_VALUE:   return DarkTheme.value;
    case COLOR_SUCCESS: return DarkTheme.success;
    case COLOR_WARNING: return DarkTheme.warning;
    case COLOR_DANGER:  return DarkTheme.danger;
    case COLOR_RESET:   return RESET_CODE;
    default:            return "";
  }
}

/**
 * @brief Retrieves the ANSI reset code.
 * @return String containing the ANSI reset code, or empty string if colors disabled.
 */
char* theme_get_reset(void)
{
  return (char*)(use_colors ? RESET_CODE : "");
}

/**
 * @brief Prints a formatted section header.
 * @param text The header text to print.
 */
void print_header(const char* text)
{
  const char* c_prim  = theme_get_color(COLOR_PRIMARY);
  const char* c_res   = theme_get_reset();

  printf("\n%s╭─ %s %s\n", c_prim, text, c_res);
}

/**
 * @brief Prints a labeled key-value field.
 * @param label The field name (left-aligned, fixed width).
 * @param format The printf-style format string for the value.
 * @param ... Additional arguments for the format string.
 */
void print_field(const char* label, const char* format, ...)
{
  const char* c_prim = theme_get_color(COLOR_PRIMARY);
  const char* c_lab  = theme_get_color(COLOR_LABEL);
  const char* c_val  = theme_get_color(COLOR_VALUE);
  const char* c_res  = theme_get_reset();

  // The %-15s guarantees all colons align perfectly vertically
  printf("%s| %s%-15s%s : %s", c_prim, c_lab, label, c_res, c_val);

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  printf("%s\n", c_res);
}

/**
 * @brief Prints a formatted section footer.
 */
void print_footer(void)
{
  const char* c_prim  = theme_get_color(COLOR_PRIMARY);
  const char* c_res   = theme_get_reset();

  printf("%s╰─%s\n", c_prim, c_res);
}

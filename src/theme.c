#include "base.h"
#include "theme.h"

#include <stdarg.h>
#include <unistd.h>

static bool use_colors = true;
static char* RESET_CODE = "\033[0m";

static const Theme DarkTheme =
{
  .primary  = "\033[1;36m", // Gray
  .label    = "\033[0;37m", // Cyan
};

void theme_init(bool force_no_color)
{
  // Crucial Low-Level Check: Is stdout a terminal or a file/pipe?
  if (force_no_color || !isatty(STDOUT_FILENO)) {
    use_colors = false;
  }
}

const char* theme_get_color(ThemeColor color)
{
  if (!use_colors)
    return NULL;

  switch (color)
  {
  case COLOR_PRIMARY:
    return DarkTheme.primary;
  
    case COLOR_LABEL:
    return DarkTheme.label;

  default:
    return NULL;
  }
}

char* theme_get_reset(void)
{
  return use_colors ? RESET_CODE : "";
}

void print_header(const char* text)
{
  const char* c_prim  = theme_get_color(COLOR_PRIMARY) ?: "";
  const char* c_res   = theme_get_reset();

  printf("\n%s╭─ %s %s\n", c_prim, text, c_res);
}

void print_field(const char* label, const char* format, ...)
{
  const char* c_lab = theme_get_color(COLOR_LABEL) ?: "";
  const char* c_val = theme_get_color(COLOR_VALUE) ?: "";
  const char* c_res = theme_get_color(COLOR_RESET) ?: "";

  // The %-15s guarantees all colons align perfectly vertically
  printf("%s| %s%-15s%s : %s", theme_get_color(COLOR_PRIMARY)?:"", c_lab, label, c_res, c_val);

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  printf("%s\n", c_res);
}

void print_footer(void)
{
  const char* c_prim  = theme_get_color(COLOR_PRIMARY) ?: "";
  const char* c_res   = theme_get_reset();

  printf("%s╰─%s\n", c_prim, c_res);
}
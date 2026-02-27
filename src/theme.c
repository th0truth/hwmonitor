#include "base.h"
#include "theme.h"

#include <unistd.h>

static bool use_colors = true;

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
  if (!use_colors) {
    return NULL;
  }

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

void print_header(const char* text)
{

}


#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <cJSON.h>

#define BUFFER_SIZE 256

#define STR_OR_UNK(s) ((s) ? (s) : "<unknown>")
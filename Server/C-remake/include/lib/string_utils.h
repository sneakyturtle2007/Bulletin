

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error_handling.h"

typedef struct{
	char *data;
  size_t length;
  size_t capacity;
} String;

Error strncpy_exclude(char **dest, const char *src, size_t dest_size, const char *exclude);

#endif
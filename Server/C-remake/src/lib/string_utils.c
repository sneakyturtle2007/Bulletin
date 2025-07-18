
#include "string_utils.h"

Error strncpy_exclude(String *dest, char *src, const char *exclude){
  if(src == NULL){
    fprintf(stderr, "ERROR: NULL source string.\n");
    return (Error) {INVALID_ARGUMENT, "string_utils.c/strncpy_exclude/ERROR: Null source string.\n"};
  }
  if(strlen(src) == 0){
    return (Error) {OK, "Success"};
  }
  char *exclude_start= strstr(src, exclude);
  char *exclude_end = exclude_start + strlen(exclude);
  char *src_end = src + strlen(src);
  printf("\nstrncpy_exclude\nexclude_start: %s\nsrc: %s\n", exclude_start, src);
  char *status = strncpy(dest->data, src, exclude_start - src);
  dest->length = (exclude_start - src);
  dest->data[dest->length] = '\0';
  if(status == NULL){
    fprintf(stderr, "ERROR: Failed to cpy src string data before exclude string to dest string.\n");
    return (Error) {STRING_ERROR,
                    "string_utils.c/strncpy_exclude/ERROR: Failed to cpy src string data before exclude string to dest string.\n"};
  }
  status = strncat(dest->data, exclude_end, src_end - exclude_end);
  if(status == NULL){
    fprintf(stderr, "ERROR: Failed to concatenate src data after exclude to dest string.\n");
    return (Error) {STRING_ERROR, 
                    "string_utils.c/strncpy_exclude/ERROR: Failed to concatenate src data after exclude to dest string./n"};
  }
  return (Error) {OK, "Success"};
}
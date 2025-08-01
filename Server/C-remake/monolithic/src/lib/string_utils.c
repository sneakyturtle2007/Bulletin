
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
  if(exclude_start == NULL){
    return (Error) {OK, "Success"};
  }
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
Error strcpy_dynamic(String *dest, const char *src){
  if(src == NULL){
    fprintf(stderr, "ERROR: NULL input.\n");
    return (Error) {INVALID_ARGUMENT, 
                    "string_utils.c/snprintf_dynamic/ERROR: Null input.\n"};
  }
  if(dest->data == NULL){
    dest->capacity = strlen(src) +1; // +1 for null termination character
    dest->data = calloc(dest->capacity, sizeof(char));
    if(dest->data == NULL){
      fprintf(stderr, "ERROR: Failed to allocate memory for dest string.\n");
      return (Error) {MEMORY_ALLOCATION_ERROR,
                      "string_utils.c/snprintf_dynamic/ERROR: Failed to allocate memory for dest string.\n"};
    }
  }
  if(dest->capacity <= strlen(src)){
    printf("src length: %d\n", strlen(src)* sizeof(char));
    dest->capacity = strlen(src) +1; // +1 for null termination character
    char *new_data = realloc(dest->data, dest->capacity);
    if(new_data == NULL){
      fprintf(stderr, "ERROR: Failed to reallocate memory for dest string.\n");
      return (Error) {MEMORY_ALLOCATION_ERROR,
                      "string_utils.c/snprintf_dynamic/ERROR: Failed to reallocate memory for dest string.\n"};
    }
    dest->data = new_data;
  }
  snprintf(dest->data, dest->capacity, "%s", src);
  dest->length = strlen(dest->data) + 1; // +1 for null termination character
  dest->data[dest->length - 1] = '\0'; // null termination character
  return (Error) {OK, "Success"};
}
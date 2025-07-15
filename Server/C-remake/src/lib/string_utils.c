
#include "string_utils.h"

Error strncpy_exclude(char **dest, const char *src, size_t dest_size, const char *exclude){
  if(dest_size < 2){
    memset(*dest, 0, dest_size);
    (*dest)[0] = '\0';
    return (Error) {OK, "Success"};
  }else if(src == NULL){
    return (Error) {INVALID_ARGUMENT, "user_management.c/strncpy_exclude/ERROR: source string null.\n"};
  }
  size_t copy_size = dest_size;
  if(strlen(src) < dest_size){
    copy_size = strlen(src) + 1; // +1 for NULL termination character
  }
  int exclude_buffer_length = strlen(exclude) + 1; // +1 for NULL termination character
  String exclude_buffer = {.data = calloc(exclude_buffer_length, sizeof(char)), .length = 0, .capacity = exclude_buffer_length};
  for(int i = 0; i <= (copy_size - 1); i++){
    if(exclude_buffer.length == exclude_buffer.capacity || i == (copy_size - 1)){
      if(strncmp(exclude_buffer.data, exclude, strlen(exclude_buffer.data)) == 0){
        continue;
      }
      char *status = strncat(*dest, src, exclude_buffer.length);
      if(status == NULL){
        fprintf(stderr, "ERROR: Failed to concatenate buffers worth of src string to dest string.\n");
        free(exclude_buffer.data);
        return (Error) {STRING_ERROR, 
                        "user_management.c/strncpy_exclude/ERROR: Failed to concatenate buffers worth of src string to dest string.\n"};
      }
      memset(exclude_buffer.data, 0, exclude_buffer.capacity);
      exclude_buffer.data[0] = '\0';
      exclude_buffer.length = 0;
    }
    exclude_buffer.data[exclude_buffer.length] = src[i];
    exclude_buffer.length += 1;
  }
  (*dest)[copy_size - 1 ] = '\0';
  free(exclude_buffer.data);
  return (Error) {OK, "Success"};
}
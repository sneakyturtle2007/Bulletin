
#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string_utils.h"
#include "error_handling.h"

typedef struct{
  String **data;
  size_t rows;
  size_t cols;
  size_t table_capacity;
} Table_String;

Error free_table(Table_String *table);

// It returns 0 on success, and non-zero on failure.
// convert_to_string_table return an int because it is used as a callback function for sqlite3_exec
int convert_to_string_table(void *data, int numCol, char **colValues, char **colNames); 

Error convert_table_to_string(Table_String *table, String *result);

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sqlite3.h>
// Ensure the C standard version is set to C23
#ifndef __STDC_VERSION__
  #define __STDC_VERSION__ 202311L  
#endif

typedef struct{
  char *data;
  size_t length;
  size_t capacity;
} String;

typedef struct{
  String **data;
  int rows;
  int cols;
  int table_capacity;
} Table_String;


int convert_to_string_table(void *data, int numCols, char **colValues, char **colNames);

int main(){
  sqlite3 *db;
  int status = sqlite3_open("database.db", &db);
  if(status != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    return 1;
  }
  printf("Database opened successfully.\n");


  Table_String result = {.data = malloc(64 * sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 1024 };

  char *errmsg;
  char *command = "PRAGMA table_info(events);";
  Table_String *result_ptr = &result;
  status = sqlite3_exec(db, command, convert_to_string_table, result_ptr, &errmsg);
  if(status != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", errmsg);
    sqlite3_free(errmsg);
    sqlite3_close(db);
    for(int i = 0; i < result.rows; i++){
      for(int k = 0; k < result.cols; k++){
        free(result.data[i][k].data); // Free string data
      }
      free(result.data[i]); // Free row of String structs
    }
    free(result.data);
    return 1;
  }

  printf("Table info retrieved successfully.\n");

  for(int i = 0; i < result.rows; i++){
    for(int k = 0; k < result.cols; k++){
      printf("%s | ", result.data[i][k].data);
    }
    printf("\n");
  }
  
  for(int i = 0; i < result.rows; i++){
    if (result.data[i]) {
        for(int k = 0; k < result.cols; k++){
            if (result.data[i][k].data)
                free(result.data[i][k].data);
        }
        free(result.data[i]);
    }
  }
  free(result.data);

  sqlite3_close(db);
  return 0;
}

int convert_to_string_table(void *data, int numCols, char **colValues, char **colNames){
  Table_String *table = (Table_String *)data;

  for(int i = 0; i < numCols; i++){
    if(colValues[i] == NULL){
      colValues[i] = "NULL"; // Handle NULL values
    }
  }

  table->data[table->rows] = malloc(numCols * sizeof(String));

  if(!table->data[table->rows]) {
    fprintf(stderr, "Memory allocation failed\n");
    return 1; // Memory allocation error
  }

  table->cols = numCols;

  for(int i = 0; i < numCols; i++){
    String temp_string = {.data = malloc(strlen(colValues[i]) + 1), .length = strlen(colValues[i]) + 1,
                          .capacity = strlen(colValues[i]) + 1};
    void *status = strncpy(temp_string.data, colValues[i], temp_string.length);
    
    if(status == NULL){
      fprintf(stderr, "ERROR: strncpy failed (function convert_to_string_table)\n");
      free(temp_string.data); // Free allocated memory before returning
      return 1; // Memory allocation error
    }

    status = memcpy(&table->data[table->rows][i], &temp_string, sizeof(temp_string));
    if(status == NULL){
      fprintf(stderr, "ERROR: memcpy failed (function convert_to_string_table)\n");
      return 1; // Memory allocation error
    }

  } 

  table->rows++;
  
  if(table->rows >= table->table_capacity) {
    table->table_capacity *= 2;
    table->data = realloc(table->data, table->table_capacity * sizeof(String*));
    if(!table->data) {
      fprintf(stderr, "Memory allocation failed\n");
      return 1; // Memory allocation error
    }
  }
  return 0;
}

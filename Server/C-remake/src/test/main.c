
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlite3.h"


typedef struct{
  char ****data;
  int rows;
  int cols;
  int capacity;
} Table_String;

int convert_table_to_string(Table_String *table, char **result);
int convert_to_string_table(void *data, int numCols, char **colValues, char **colNames);
int open_database(sqlite3 **db);
int describe_table(sqlite3 **db, char *name, Table_String *description);

int main(){
  sqlite3 *db;
  int status = open_database(&db);
   
  if(status != SQLITE_OK){
    fprintf(stderr, "ERROR: Failed to open database\n");
    return 1;
  }
  Table_String table = {.data = malloc(1024 * sizeof(char)), .rows = 0,
                        .cols = 0, .capacity = 10};
  status = describe_table(&db, "events", &table);
  if(status != 0){
    fprintf(stderr, "ERROR: failed with error code %d\n", status);
    return 1;
  }/*
  char *result = malloc(512 * sizeof(char));
  status = convert_table_to_string(&table, &result);
  if(status != 0){
    fprintf(stderr, "ERROR: failed to convert table to string\n");
    return 1;
  }
  printf("Events table: %s\n", result);

  free(result);
  free(table.data);
  */
  return 0;
}

int convert_table_to_string(Table_String *table, char **result){
  char *state;
  for(int i = 0; i < table->rows; i++){
    for(int k = 0; k < table->cols; i++){
      char *cpytext = table->data[i][k];

      state = strncat(cpytext, "|", 2);
      if(state == NULL){
        fprintf(stderr, "ERROR: Failed to convert table to string (failed to concatenate separator symbol to column at row %d col %d\n", i, k);
        return 7;
      }

      state = strncat(*result, cpytext, strlen(cpytext));
      if(state == NULL){
        fprintf(stderr, "ERROR: Failed to convert table to string (failed to concatenate column %d on row %d to result string)\n", i, k);
        return 8;
      }
    }
    state = strncat(*result, "\n", 2);
    if(state == NULL){
      fprintf(stderr, "ERROR: Failed to convert table to string (failed to concatenate new line character onto end of row %d)\n", i);
      return 9;
    }
  }
  state = strncat(*result, "\0", 2);
  if(state ==NULL){
    fprintf(stderr, "ERROR: Failed to convert table to string (failed to concatenate terminating character");
    return 11;
  }
  return 0;
}

int convert_to_string_table(void *data, int numCols, char **colValues, char **colNames){
  Table_String *table = (Table_String*) data;

  printf("Number of columns %d\n", numCols);
  if(table == NULL){
    fprintf(stderr, "ERROR: Table pointer is null\n");
    return 3;
  }
  if(table->rows == 0){
    table->cols = numCols;
  }else if(table->cols != numCols){
    fprintf(stderr, "ERROR: Inconsistent column count (expected %d, got %d).\n", table->cols, numCols);
    return 4;
  }
  if(table->rows >= table->capacity){
    int new_capacity = table->capacity * 2;
    char ****data_adjusted = realloc(table->data, new_capacity * sizeof(char***));
    if(data_adjusted == NULL){
      fprintf(stderr, "ERROR: Failed to reallocate more memory for table data\n");
      return 5;
    }
    table->data = data_adjusted;
    table->capacity = new_capacity;
    free(data_adjusted);
  }
  char ***data_adjusted = malloc(numCols * sizeof(char***));
  if(data_adjusted == NULL){
    fprintf(stderr, "ERROR: Failed to allocated memory for row %d\n", table->rows);
    return 6;
  }
  table->data[table->rows] = data_adjusted;
  for(int i = 0; i < table->cols; i++){
    table->data[table->rows][i][table->rows] = strndup(colValues[i], strlen(colValues[i]));
    printf("column value %s\n", table->data[table->rows][i][table->rows);

  }
  table->rows++;
  free(data_adjusted);
  return 0;
}

int describe_table(sqlite3 **db, char *name, Table_String *description){

  char command[64];

  sprintf(command, "PRAGMA table_info(%s);\n", name);

  printf("%s\n", command);

  char *errmsg;

  int state = sqlite3_exec(*db, command, convert_to_string_table, description, &errmsg);

  printf("state code %d\n", state);

  if(state != SQLITE_OK){
    fprintf(stderr, "ERROR: Failed to describe table");
    return state;
  }
  return SQLITE_OK;
}

int open_database(sqlite3 **db){
  const int rc = sqlite3_open("./database.db", db);
  if(rc){
    printf("ERROR: Failed to open database\n");
    sqlite3_close(*db);
    return 1;
  }
  printf("Database opened\n");
  return 0;
}

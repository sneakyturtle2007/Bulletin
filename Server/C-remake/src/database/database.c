#include "database.h"
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int open_database(sqlite3 **db){
  const int rc = sqlite3_open("database.db", db);
  if(rc){
    printf("ERROR: Failed to open database\n");
    sqlite3_close(*db);
    return 1;
  }
  printf("Database opened\n");
}

int get_tables(sqlite3 **db, Table_String *tables){
  
  char *command = "SELECT name FROM sqlite_master WHERE type=table";

  char *errmsg; 

  int status = sqlite3_exec(*db, command, convert_to_string_table, tables, &errmsg);
  if(status != SQLITE_OK){
    fprintf(stderr, "ERROR: Failed to get tables\n%s", errmsg);
    return 2; 
  }
  return SQLITE_OK;
}

// Table Management
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
    char ***data_adjusted = realloc(table->data, new_capacity * sizeof(char**));
    if(data_adjusted == NULL){
      fprintf(stderr, "ERROR: Failed to reallocate more memory for table data\n");
      return 5;
    }
    table->data = data_adjusted;
    table->capacity = new_capacity;
    free(data_adjusted);
  }
  char **data_adjusted = malloc(numCols * sizeof(char**));
  if(data_adjusted == NULL){
    fprintf(stderr, "ERROR: Failed to allocated memory for row %d\n", table->rows);
    return 6;
  }
  table->data[table->rows] = data_adjusted;
  for(int i = 0; i < table->cols; i++){
    table->data[table->rows][i] = strndup(colValues[i], strlen(colValues[i])); 
    printf("column value %s\n", table->data[table->rows][i]);

  }
  table->rows++;
  free(data_adjusted);
  return 0;
}


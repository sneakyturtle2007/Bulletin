#include "database.h"
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  char ***data;
  int rows;
  int cols;
  int capacity;
} Table_String;

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
  char *type = "table";
  const char *command;
  sprintf(command, "%s%s\n","SELECT name FROM sqlite_master WHERE type=", type);

  char *errmsg; 

  int status = sqlite3_exec(db, command, convert_to_string_table, tables, &errmsg);
  if(status != SQLITE_OK){
    fprintf(stderr, "ERROR: Failed to get tables\n%s", errmsg);
    return 2; 
  }
  return SQLITE_OK;
}

// Table Management
int describe_tables(sqlite3 **db, char *name, Table_String *description){
  const char *command;
  sprintf(command, "PRAGMA table_info(%s);\n", command);

  char *errmsg;

  int state = sqlite3_exec(*db, command, convert_to_string_table, description, &errmsg);

  if(state != SQLITE_OK){
    fprintf(stderr, "ERROR: Failed to describe table");
    return state;
  }
  return SQLITE_OK;
}


int convert_to_string_table(void *data, int numCol, char **colValues, char **colNames){
  Table_String *table = (Table_String*) data;
  if(table == NULL){
    fprintf(stderr, "ERROR: Table pointer is null\n");
    return 3;
  }
  if(table->rows == 0){
    table->cols = numCols;
  }else if(table->cols != numcol){
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
  }
  char ***data_adjusted = malloc(numCol * sizeof(Char**));
  if(data_adjusted == NULL){
    fprintf(stderr, "ERROR: Failed to allocated memory for row %d\n", table->rows);
    return 6;
  }
  table->data[table->rows] = data_adjusted;
  for(int i = 0; i < table->cols; i++){
    strdup(table->data[table->rows][i], colValues[i]); 
  }
  table->rows ++;
  return 0;
}


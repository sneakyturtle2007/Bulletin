#include "../HeaderFiles/database.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  char ***data;
  int row;
  int cols;
  int capacity;
} Table_String;

void open_database(sqlite3 **db){
  const int rc = sqlite3_open("database.db", db);
  if(rc){
    printf("error opening database\n");
    sqlite3_close(*db);
    exit(EXIT_FAILURE);
  }
  printf("opened database\n");
}

int get_tables(sqlite3 **db){
  char *type = "table";
  const char *command;
  sprintf(command, "%s%s\n","SELECT name FROM sqlite_master WHERE type=", type);
  int status = sqlite3_exec(db, command,0,0);
  if(status != SQLITE_OK){
    return 0;
  }
}
// Table Management
int describe_table(char *name, char **description){
  const char *command;
  sprintf(command, "PRAGMA table_info(%s);\n", command);
  
}

int callback(void *data, int numCol, char **colValues, char **colNames ){
   
}

int convert_to_string_table(void *data, int numCol, char **colValues, char **colNames){
  Table_String *table = (*Table_String) data;

  if(table->row == 0){
    table->cols = numCols;
  }

  if(table->row >= table->capacity){
    int new_capacity = table->capacity * 2;
    table->data = realloc(table->data, new_capacity * sizeof(char**));
    table->capacity = new_capacity;
  }

  table->data[table->row] = malloc(numCol * sizeof(char**));
  
  for(int i = 0; i < table->cols; i++){
    table->data[table->row][i] = colValues[i];
  }

  table->row ++;
  return 0;
}


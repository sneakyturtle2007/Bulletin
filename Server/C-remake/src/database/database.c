
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "database.h"



int open_database(sqlite3 **db){
  const int rc = sqlite3_open("database.db", db);
  if(rc){
    printf("ERROR: Failed to open database\n");
    sqlite3_close(*db);
    return 1;
  }
  printf("Database opened\n");
  return 0;
}


// Table Management

  int get_tables(sqlite3 **db, Table_String *tables){
    char *command = "SELECT name FROM sqlite_master WHERE type=table";
    char *errmsg; 
    int status = sqlite3_exec(*db, command, convert_to_string_table, tables, &errmsg);
    if(status != SQLITE_OK){
      fprintf(stderr, "ERROR: Failed to get tables\n%s", errmsg);
      sqlite3_free(errmsg);
      free_table(tables);
      return 2; 
    }
    return SQLITE_OK;
  }

  int describe_table(sqlite3 **db, char *name, Table_String *description){
    char command[64];
    sprintf(command, "PRAGMA table_info(%s);", name);
    char *errmsg;
    int state = sqlite3_exec(*db, command, convert_to_string_table, description, &errmsg);
    if(state != 0){
      fprintf(stderr, "ERROR: Failed to describe table");
      free_table(description);
      sqlite3_free(errmsg);
      return state;
    }
    return 0;
  }

  int create_table(sqlite3 **db, char *name, char *columns){
    char command[512];
    sprintf(command, "CREATE TABLE IF NOT EXISTS %s (%s);", name, columns);
    printf("%s\n", command);
    char *errmsg;
    int state = sqlite3_exec(*db, command, NULL, NULL, &errmsg);
    if(state != SQLITE_OK){
      fprintf(stderr, "ERROR: Failed to create table %s\n%s", name, errmsg);
      sqlite3_free(errmsg);
      return 14; 
    }
    return 0; 
  }

  int drop_table(sqlite3 **db, char *name){
    char command[128];
    sprintf(command, "DROP TABLE IF EXISTS %s;", name);
    printf("%s\n", command);
    char *errmsg;
    int state = sqlite3_exec(*db, command, NULL, NULL, &errmsg);
    if(state != SQLITE_OK){
      fprintf(stderr, "ERROR: Failed to drop table %s\n%s", name, errmsg);
      sqlite3_free(errmsg);
      return 15; 
    }
    return 0;
  }

  // Info Management
    int delete_from_table(sqlite3 **db, char *name, char *condition){
      char command[512];
      sprintf(command, "DELETE FROM %s WHERE %s;", name, condition);
      char *errmsg;
      int state = sqlite3_exec(*db, command, NULL, NULL, &errmsg);
      if(state != SQLITE_OK){
        fprintf(stderr, "ERROR: Failed to delete from table %s\n%s", name, errmsg);
        sqlite3_free(errmsg);
        return 16;
      }
      return 0;
    }

    int insert_into_table(sqlite3 **db, char *name, char *variables, char *values){
      char command[512];
      sprintf(command, "INSERT INTO %s (%s) VALUES (%s);", name, variables, values);
      printf("%s\n", command);
      char *errmsg;
      int state = sqlite3_exec(*db, command, NULL, NULL, &errmsg);
      if(state != SQLITE_OK){
        fprintf(stderr, "ERROR: Failed to insert into table %s\n%s", name, errmsg);
        sqlite3_free(errmsg);
        return 17; 
      }
      return 0;
    }

    int update_table_info(sqlite3 **db, char *name, char *variables, char *values){
      char command[1024];
      sprintf(command, "UPDATE %s SET %s WHERE %s;", name, variables, values);
      printf("%s\n", command);
      char *errmsg;
      int state = sqlite3_exec(*db, command, NULL, NULL, &errmsg);
      if(state != SQLITE_OK){
        fprintf(stderr, "ERROR: Failed to update table %s\n%s", name, errmsg);
        sqlite3_free(errmsg);
        return 18; 
      }
    }
// Utility Functions
  int free_table(Table_String *table){
    if(table == NULL || table->data == NULL) {
      return 0;
    }
    for(int i = 0; i < table->rows; i++){
      if(table->data[i] != NULL) {
        for(int j = 0; j < table->cols; j++){
          free(table->data[i][j].data); // Free string data
        }
        free(table->data[i]); // Free row of String structs
      }
    }
    free(table->data);
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "database.h"



Error open_database(sqlite3 **db){
  const int rc = sqlite3_open("database.db", db);
  if(rc){
    printf("ERROR: Failed to open database\n");
    sqlite3_close(*db);
    return (Error) {DATABASE_ERROR, 
                        "database.c/open_database/ERROR: Failed to open database.\n" };
  }
  printf("Database opened\n");
  return (Error) {OK, "Database opened successfully"};
}


// Table Management

  Error get_tables(sqlite3 **db, Table_String *tables){
    char *command = "SELECT name FROM sqlite_master WHERE type=table";
    char *errmsg; 
    int status = sqlite3_exec(*db, command, convert_to_string_table, tables, &errmsg);
    if(status != SQLITE_OK){
      fprintf(stderr, "ERROR: Failed to get tables\n%s", errmsg);
      sqlite3_free(errmsg);
      free_table(tables);
      return (Error) {DATABASE_ERROR, 
                        "database.c/get_tables/ERROR: Failed to get tables.\n" };
    }
    return (Error) {OK, "Success"};
  }

  Error describe_table(sqlite3 **db, char *name, Table_String *description){
    int command_length = strlen(name) + 64;
    char *command = malloc(command_length * sizeof(char));
    sprintf(command, "PRAGMA table_info(%s);", name);
    char *errmsg;
    int state = sqlite3_exec(*db, command, convert_to_string_table, description, &errmsg);
    free(command);
    if(state != 0){
      fprintf(stderr, "ERROR: Failed to describe table");
      free_table(description);
      sqlite3_free(errmsg);
      return (Error) {DATABASE_ERROR, 
                        "database.c/describe_table/ERROR: Failed to describe table.\n"};
    }
    return (Error) {OK, "Success"};
  }

  Error create_table(sqlite3 **db, char *name, char *columns){
    int command_length = strlen(name) + strlen(name) + strlen(columns) + 64;
    char *command = malloc(command_length * sizeof(char));
    sprintf(command, "CREATE TABLE IF NOT EXISTS %s (%s);", name, columns);
    printf("%s\n", command);
    char *errmsg;
    int state = sqlite3_exec(*db, command, NULL, NULL, &errmsg);
    if(state != SQLITE_OK){
      fprintf(stderr, "ERROR: Failed to create table %s\n%s", name, errmsg);
      sqlite3_free(errmsg);
      return (Error) {DATABASE_ERROR, 
                        "database.c/create_table/ERROR: Failed to create table.\n"}; 
    }
    return (Error) {OK, "Success"}; 
  }

  Error drop_table(sqlite3 **db, char *table_name){
    int command_length = strlen(table_name) + 64;
    char *command = malloc(command_length * sizeof(char));
    sprintf(command, "DROP TABLE IF EXISTS %s;", table_name);
    printf("%s\n", command);
    char *errmsg;
    int state = sqlite3_exec(*db, command, NULL, NULL, &errmsg);
    free(command);
    if(state != SQLITE_OK){
      fprintf(stderr, "ERROR: Failed to drop table %s\n%s", table_name, errmsg);
      sqlite3_free(errmsg);
      return (Error) {DATABASE_ERROR, 
                        "database.c/drop_table/ERROR: Failed to drop table.\n" };
    }
    return (Error) {OK, "Success"};
  }

  // Info Management
    Error get_from_table(sqlite3 **db, char *table_name, char *condition, Table_String *result){
      int command_length = strlen(table_name) + strlen(condition) + 64; // +64 overhead for formatting
      char *command = malloc(command_length * sizeof(char));
      sprintf(command, "SELECT * FROM %s WHERE %s;", table_name, condition);
      char *errmsg;
      int state = sqlite3_exec(*db, command, convert_to_string_table, result, &errmsg);
      free(command);
      if(state != SQLITE_OK){
        fprintf(stderr, "ERROR: Failed to get from table %s\n%s", table_name, errmsg);
        sqlite3_free(errmsg);
        return (Error) {DATABASE_ERROR, 
                        "database.c/get_from_table/ERROR: Failed to get from table.\n" }; 
      }
      return (Error) {OK, "Success"};
    }

    Error delete_from_table(sqlite3 **db, char *table_name, char *condition){
      int command_length = strlen(table_name) + strlen(condition) + 64; // +64 overhead for formatting
      char *command = malloc(command_length * sizeof(char));
      sprintf(command, "DELETE FROM %s WHERE %s;", table_name, condition);
      char *errmsg;
      int state = sqlite3_exec(*db, command, NULL, NULL, &errmsg);
      free(command); 
      if(state != SQLITE_OK){
        fprintf(stderr, "ERROR: Failed to delete from table %s\n%s", table_name, errmsg);
        sqlite3_free(errmsg);
        return (Error){DATABASE_ERROR, 
                        "database.c/delete_from_table/ERROR: Failed to delete from table.\n"};
      }
      return (Error) {OK, "Success"};
    }

    Error insert_into_table(sqlite3 **db, char *table_name, char *variables, char *values){
      int command_length = strlen(table_name) + strlen(variables) + strlen(values) + 64;
      char *command = malloc(command_length * sizeof(char));
      sprintf(command, "INSERT INTO %s (%s) VALUES (%s);", table_name, variables, values);
      printf("%s\n", command);
      char *errmsg;
      int state = sqlite3_exec(*db, command, NULL, NULL, &errmsg);
      free(command); 
      if(state != SQLITE_OK){
        fprintf(stderr, "ERROR: Failed to insert into table %s\n%s", table_name, errmsg);
        sqlite3_free(errmsg);
        return (Error) {DATABASE_ERROR, 
                        "database.c/insert_into_table/ERROR: Failed to insert into table.\n" }; 
      }
      return (Error) {OK, "Success"};
    }

    Error update_table_info(sqlite3 **db, char *table_name, char *variables_and_values, char *condition){
      int command_length = strlen(table_name) + strlen(variables_and_values) + strlen(condition) + 64;
      char *command = malloc(command_length * sizeof(char));
      sprintf(command, "UPDATE %s SET %s WHERE %s;", table_name, variables_and_values, condition);
      printf("%s\n", command);
      char *errmsg;
      int state = sqlite3_exec(*db, command, NULL, NULL, &errmsg);
      free(command);
      if(state != SQLITE_OK){
        fprintf(stderr, "ERROR: Failed to update table %s\n%s", table_name, errmsg);
        sqlite3_free(errmsg);
        return (Error) {DATABASE_ERROR, 
                        "database.c/update_table_info/ERROR: Failed to update table information.\n"}; 
      }
      return (Error) {OK, "Success"};
    }

// Utility Functions
  Error free_table(Table_String *table){
    if(table == NULL || table->data == NULL) {
      return (Error) {OK, "Nothing to free\n"};
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
    table->rows = 0;
    table->cols = 0;
    table->table_capacity = 0;
    return (Error) {OK, "Memory freed\n"};
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
      return 1;
    }

    table->cols = numCols;

    for(int i = 0; i < numCols; i++){
      String temp_string = {.data = malloc(strlen(colValues[i]) + 1), .length = strlen(colValues[i]) + 1,
                            .capacity = strlen(colValues[i]) + 1};

      void *status = strncpy(temp_string.data, colValues[i], temp_string.length);
      if(status == NULL){
        fprintf(stderr, "ERROR: strncpy failed (function convert_to_string_table)\n");
        free(temp_string.data); 
        return 1; 
      }

      //status = memcpy(&table->data[table->rows][i], &temp_string, sizeof(temp_string));
      table->data[table->rows][i] = temp_string;

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
    return 0; // Success
  }

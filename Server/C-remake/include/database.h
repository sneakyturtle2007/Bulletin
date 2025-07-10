//
// Created by luis on 6/6/25
//



#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sqlite3.h"
#include "error_handling.h"

typedef struct{
  char *data;
  size_t length;
  size_t capacity;
} String;

typedef struct{
  String **data;
  size_t rows;
  size_t cols;
  size_t table_capacity;
} Table_String;

// Database Management
  /**
   * Opens the database and returns a pointer to the database connection.
   * @param db Pointer to a sqlite3 pointer that will hold the database connection.
   * @return 0 on success, non-zero error code on failure.
   */

Error open_database(sqlite3 **db);

// Table Management
  Error get_tables(sqlite3 **db, Table_String *tables);

  Error describe_table(sqlite3 **db, char *name, Table_String *description);

  Error create_table(sqlite3 **db, char *name, char *columns);

  Error drop_table(sqlite3 **db, char *name);

  // Data Management
    Error get_from_table(sqlite3 **db, char *name, char *condition, Table_String *result);

    Error delete_from_table(sqlite3 **db, char *name, char *condition);

    Error insert_into_table(sqlite3 **db, char *name, char *variables, char *values);

    Error update_table_info(sqlite3 **db, char *name, char *variables, char *values);

// Utility Functions
  Error free_table(Table_String *table);

  int convert_to_string_table(void *data, int numCol, char **colValues, char **colNames);

#endif 

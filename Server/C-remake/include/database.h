//
// Created by luis on 6/6/25
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlite3.h"

#ifndef DATABASE_H
#define DATABASE_H

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
int open_database(sqlite3 **db);

// Table Management
  int get_tables(sqlite3 **db, Table_String *tables);

  int describe_table(sqlite3 **db, char *name, Table_String *description);

  int create_table(sqlite3 **db, char *name, char *columns);

  int drop_table(sqlite3 **db, char *name);

  // Data Management
    int delete_from_table(sqlite3 **db, char *name, char *condition);

    int insert_into_table(sqlite3 **db, char *name, char *variables, char *values);

    int update_table_info(sqlite3 **db, char *name, char *variables, char *values);

// Utility Functions
  int free_table(Table_String *table);

  int convert_to_string_table(void *data, int numCol, char **colValues, char **colNames);

#endif 

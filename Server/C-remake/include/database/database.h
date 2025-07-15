//
// Created by luis on 6/6/25
//



#ifndef DATABASE_H
#define DATABASE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "sqlite3.h"
#include "error_handling.h"
#include "string_utils.h"
#include "table.h"


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
    Error get_from_table(sqlite3 **db, char* operation, char *name, char *condition, Table_String *result);

    Error delete_from_table(sqlite3 **db, char *name, char *condition);

    Error insert_into_table(sqlite3 **db, char *name, char *variables, char *values);

    Error update_table_info(sqlite3 **db, char *name, char *variables, char *values);


#endif 

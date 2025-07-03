//
// Created by luis on 6/6/25
//
#include "sqlite3.h"

#ifndef DATABASE_H
#define DATABASE_H

typedef struct{
  char ***data;
  int rows;
  int cols;
  int capacity;
} Table_String;

int open_database(sqlite3 **db);

int describe_table(sqlite3 **db, char *name, Table_String *description);

int convert_to_string_table(void *data, int numCol, char **colValues, char **colNames);

#endif 

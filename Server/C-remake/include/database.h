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



int open_database(sqlite3 **db);

int describe_table(sqlite3 **db, char *name, Table_String *description);

int free_table(Table_String *table);

int convert_to_string_table(void *data, int numCol, char **colValues, char **colNames);

#endif 

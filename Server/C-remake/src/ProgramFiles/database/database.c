#include "../HeaderFiles/database.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

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
  char **result = (char **)data;

}

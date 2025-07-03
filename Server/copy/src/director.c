//
// Created by luis on 6/4/25.
//

#include "director.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "database.h"

int convert_table_to_string(Table_String *table, char **result);

int input_handler(sqlite3 **db, char* input, char **output) {
  char* inputToken = strtok(input, "|");
  if (strcmp(inputToken, "createuser")) {
    *output = "CreateUser";  
    return 0;
  }else if (strcmp(inputToken, "login")) {
    *output = "Login";
    return 0;
  }else if(strcmp(inputToken, "deleteuser")) {
    *output = "DeleteUser";
    return 0;
  }else if(strcmp(inputToken, "test")){
        
      Table_String table = {.rows = 0, .cols = 0, .capacity = 0};
      int state = describe_table(db, "events", &table);
      
      if(state != SQLITE_OK){
        return 1;
      }
      state = convert_table_to_string(&table, output);
      if(state != SQLITE_OK){
        return 1;
      }
      return SQLITE_OK;
  }else{
      *output = "Invalid Command";
      return 1;
  }
}

int convert_table_to_string(Table_String *table, char **result){
  char *state;
  for(int i = 0; i < table->rows; i++){
    for(int k = 0; k < table->cols; i++){
      char *cpytext = table->data[i][k];
      
      state = strncat(cpytext, "|", 2);
      if(state == NULL){
        fprintf(stderr, "ERROR: Failed to convert table to string (failed to concatenate separator symbol to column at row %d col %d\n", i, k);
        return 7;
      }
      
      state = strncat(*result, cpytext, strlen(cpytext));
      if(state == NULL){
        fprintf(stderr, "ERROR: Failed to convert table to string (failed to concatenate column %d on row %d to result string)\n", i, k);
        return 8;
      }
    }
    state = strncat(*result, "\n", 2);
    if(state == NULL){
      fprintf(stderr, "ERROR: Failed to convert table to string (failed to concatenate new line character onto end of row %d)\n", i);
      return 9;
    }
  }
  return 0;
}


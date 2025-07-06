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
  char *inputToken = strtok(input, "|");
  //char *state = strncat(inputToken, "\0", 2);
  char *state;
  if(state == NULL){
    fprintf(stderr, "ERROR: Failed to concatenate terminating symbol on input");
    return 12;
  }
  if (strcmp(inputToken, "createuser") == 0) {
    printf("%s\n", inputToken);
    *output = "CreateUser";  
    return 0;
  }else if (strcmp(inputToken, "login") == 0) {
    *output = "Login";
    return 0;
  }else if(strcmp(inputToken, "deleteuser") == 0) {
    *output = "DeleteUser";
    return 0;
  }else if(strcmp(inputToken, "test") == 0){
        
    Table_String table = {.data = malloc(50 * sizeof(char **)), .rows = 0, .cols = 0, .capacity = 0};
    int state = describe_table(db, "events", &table);
      
    if(state != SQLITE_OK){
      return state;
    }
    state = convert_table_to_string(&table, output);
    free(table.data);
    if(state != SQLITE_OK){
      return state;
    }
    return SQLITE_OK;
  }else{
    printf("Cause of ERROR --> %s | ERROR: Invalid command\n ", inputToken);
    return 10;
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
  state = strncat(*result, "\0", 2);
  if(state ==NULL){
    fprintf(stderr, "ERROR: Failed to convert table to string (failed to concatenate terminating character");
    return 11;
  }
  return 0;
}


//
// Created by luis on 6/4/25.
//


#include "database.h"
#include "user_management.h"

Error convert_table_to_string(Table_String *table, String *result);

Error input_handler(sqlite3 **db, char* input, String *output) {
  char *inputToken = strtok(input, "|");
  char *state = strncat(inputToken, "\0", 2);
  if(state == NULL){
    fprintf(stderr, "ERROR: Failed to concatenate terminating symbol on input");
    return (Error) {STRING_ERROR, 
                    "director.c/input_handler/ERROR: Failed to concatenate terminating symbol on input.\n"};
  }
  if (strcmp(inputToken, "createuser") == 0) {
    printf("%s\n", inputToken);
    char *username = strtok(NULL, "|");
    char *email = strtok(NULL, "|");
    char *password = strtok(NULL, "|");
    if(username == NULL || email == NULL || password == NULL) {
      fprintf(stderr, "ERROR: Missing parameters for createuser command\n");
      return (Error) {INVALID_ARGUMENT, 
                      "director.c/input_handler/ERROR: Missing parameters for createuser command.\n"};
    }
    Error status = create_user(db, username, email, password);
    if(status.code != OK){
      fprintf(stderr, "ERROR: Failed to create user\n");
      return status;
    }
    strcpy(output->data, "Success");
    output->length = strlen(output->data); 
    output->data[output->length] = '\0';
    return status;

  }else if(strcmp(inputToken, "deleteuser") == 0) {
    printf("%s\n", inputToken);
    char *userID = strtok(NULL, "|");
    if(userID == NULL) {
      fprintf(stderr, "ERROR: Missing userID for deleteuser command\n");
      return (Error) {INVALID_ARGUMENT, 
                      "director.c/input_handler/ERROR: Missing userID for deleteuser command.\n"};
    }
    Error status = delete_user(db, userID);
    if(status.code != OK){
      fprintf(stderr, "ERROR: Failed to delete user\n");
    }
    strcpy(output->data, "Success");
    output->length = strlen(output->data);
    output->data[output->length] = '\0';
    return status;

  }else if (strcmp(inputToken, "login") == 0) {
    printf("%s\n", inputToken);
    char *username = strtok(NULL, "|");
    char *password = strtok(NULL, "|");
    if(username == NULL || password == NULL) {
      fprintf(stderr, "ERROR: Missing parameters for login command\n");
      return (Error) {INVALID_ARGUMENT, 
                      "director.c/input_handler/ERROR: Missing parameters for login command.\n"};
    }
    Error status = login(db, username, password, output);
    if(status.code != OK){
      fprintf(stderr, "ERROR: Failed to login user\n");
    }
    return status;

  }else if(strcmp(inputToken, "addfriend") == 0) {
    printf("%s\n", inputToken);
    char *user_id = strtok(NULL, "|");
    char *friend_username = strtok(NULL, "|");
    if(user_id == NULL || friend_username == NULL){
      fprintf(stderr, "ERROR: Missing parameters for addfriend command.\n");
      return (Error) {INVALID_ARGUMENT,
                      "director.c/input_handler/ERROR: Missing parameters for addfriend command.\n"};
    }
    Error status = add_friend(db, user_id, friend_username);
    if(status.code != OK){
      fprintf(stderr, "ERROR: Failed to add friend.\n");
    }
    strcpy(output->data, "Success");
    output->length = strlen(output->data);
    output->data[output->length] = '\0';
    return status;

  }else if (strcmp(inputToken, "removefriend") == 0){
    printf("%s\n", inputToken);
    char *user_id = strtok(NULL, "|");
    char *friend_username = strtok(NULL, "|");
    if(user_id == NULL || friend_username == NULL){
      fprintf(stderr, "ERROR: Missing parameters for removefriend command.\n");
      return (Error) {INVALID_ARGUMENT,
                      "director.c/input_handler/ERROR: Missing parameters for removefriend command.\n"};
    }
    Error status = remove_friend(db, user_id, friend_username);
    if(status.code != OK){
      fprintf(stderr, "ERROR: Failed to remove friend.\n");
    }
    strcpy(output->data, "Success");
    output->length = strlen(output->data);
    output->data[output->length] = '\0';
    return status;

  }else if(strcmp(inputToken, "test") == 0){
    Table_String table = {.data = malloc(64 * sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 64};
    Error state = describe_table(db, "events", &table);
    printf("Testing\n");
    if(state.code != OK){
      free_table(&table);
      fprintf(stderr, "ERROR: Failed to describe table\n");
      return state;
    }
    state = convert_table_to_string(&table, output);
    if(state.code != OK){
      free_table(&table);
      fprintf(stderr, "ERROR: Failed to convert table to string\n");
      return state;
    }
    free_table(&table);
    return (Error){OK, "Success.\n"};
  }else{
    printf("Cause of ERROR --> %s | ERROR: Invalid command\n ", inputToken);
    return (Error) {INVALID_ARGUMENT,
                    "director.c/input_handler/ERROR: Invalid Command.\n"};
  }
}

Error convert_table_to_string(Table_String *table, String *result){
  char *state;
  for(int i = 0; i < table->rows; i++){
    for(int k = 0; k < table->cols; k++){
      if(result->capacity < result->length + strlen(table->data[i][k].data) + 2){
        result->capacity *= 2;
        char *new_data = realloc(result->data, result->capacity);
        if(new_data == NULL){
          fprintf(stderr, "ERROR: Failed to reallocate more memory for string data\n");
          return (Error) {MEMORY_ALLOCATION_ERROR, 
                          "director.c/convert_table_to_string/ERROR: Failed to reallocate more memory for result string.\n"};
        }
        result->data = new_data;
      }
      state = strncat(result->data, table->data[i][k].data, strlen(table->data[i][k].data));
      if(state == NULL){
        fprintf(stderr, "ERROR: failed to concatenate table data to resulting string\n");
        return (Error) {STRING_ERROR, 
                        "director.c/convert_table_to_string/ERROR: Failed to concatenate table data to resulting string.\n"};
      }
      state = strncat(result->data, "|", 2);
      if(state == NULL){
        fprintf(stderr, "ERROR: failed to concatenate separating symbol to resulting string\n");
        return (Error) {STRING_ERROR, 
                        "`director.c/convert_table_to_string/ERROR: Failed to concatenate column separator symbol.\n"};
      }
      result->length += (strlen(table->data[i][k].data) + 1);
    }
    state = strncat(result->data, "\n", 2);
    result->length += 1; // Increment length for the new line character
    if(state == NULL){
      fprintf(stderr, "ERROR: Failed to convert table to string (failed to concatenate new line character onto end of row %d)\n", i);
      return (Error) {STRING_ERROR,
                      "director.c/convert_table_to_string/ERROR: Failed to concatenate new line character onto end of row.\n"};
    }
  }
  return (Error) {OK, "Success.\n"};
}


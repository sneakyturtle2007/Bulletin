//
// Created by luis on 6/4/25.
//

#include "director.h"


Error input_handler(sqlite3 **db, char* input, String *output) {
  char *inputToken = strtok(input, "|");

  char *state = strncat(inputToken, "\0", 2);
  if(state == NULL){
    fprintf(stderr, "ERROR: Failed to concatenate terminating symbol on input");
    return (Error) {STRING_ERROR, 
                    "director.c/input_handler/ERROR: Failed to concatenate terminating symbol on input.\n"};
  }

  if (strcmp(inputToken, "createuser") == 0) {

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

    Table_String table = {.data = calloc(64, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 64};
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


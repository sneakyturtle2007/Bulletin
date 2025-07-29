//
// Created by luis on 6/4/25.
//

#include "director.h"

// Function declarations 

Error prepare_and_exec_create_user(sqlite3 **db, char *input, String *output);

Error prepare_and_exec_delete_user(sqlite3 **db, char *input, String *output);

Error prepare_and_exec_login(sqlite3 **db, char *input, String *output);

Error prepare_and_exec_add_friend(sqlite3 **db, char *input, String *output);

Error prepare_and_exec_remove_friend(sqlite3 **db, char *input, String *output);

Error prepare_and_exec_create_event(sqlite3 **db, char *input, String *output);

Error prepare_and_exec_delete_event(sqlite3 **db, char *input, String *output);

Error prepare_and_exec_add_invitee(sqlite3 **db, char *input, String *output);

Error prepare_and_exec_remove_invitee(sqlite3 **db, char *input, String *output);

Error prepare_and_exec_remove_multiple_invitees(sqlite3 **db, char *input, String *output);

Error prepare_and_exec_get_month_events(sqlite3 **db, char *input, String *output);

Error error_to_json(Error status, String *output);

// Function definitions

Error input_handler(sqlite3 **db, char* input, String *output) {
  char *inputToken = strtok(input, "|");

  char *state = strncat(inputToken, "\0", 2);
  if(state == NULL){
    fprintf(stderr, "ERROR: Failed to concatenate terminating symbol on input");
    return (Error) {STRING_ERROR, 
                    "director.c/input_handler/ERROR: Failed to concatenate terminating symbol on input.\n"};
  }

  if (strcmp(inputToken, "createuser") == 0) {
    Error status = prepare_and_exec_create_user(db, input, output);
    return error_to_json(status, output);

  }else if(strcmp(inputToken, "deleteuser") == 0) {
    Error status = prepare_and_exec_delete_user(db, input, output);
    return error_to_json(status, output);

  }else if (strcmp(inputToken, "login") == 0) {
    Error status = prepare_and_exec_login(db, input, output);
    return error_to_json(status, output);

  }else if(strcmp(inputToken, "addfriend") == 0) {
    Error status = prepare_and_exec_add_friend(db, input, output);
    return error_to_json(status, output);

  }else if (strcmp(inputToken, "removefriend") == 0){
    Error status = prepare_and_exec_remove_friend(db, input, output);
    return error_to_json(status, output);

  }else if(strcmp(inputToken, "createevent") == 0){
    Error status = prepare_and_exec_create_event(db, input, output);
    return error_to_json(status, output);

  } else if(strcmp(inputToken, "deleteevent") == 0){
    Error status = prepare_and_exec_delete_event(db, input, output);
    return error_to_json(status, output);

  }else if(strcmp(inputToken, "addinvitee") == 0){
    Error status = prepare_and_exec_add_invitee(db, input, output);
    return error_to_json(status, output);

  }else if(strcmp(inputToken, "removeinvitee") == 0){
    Error status = prepare_and_exec_remove_invitee(db, input, output);
    return error_to_json(status, output);

  }else if(strcmp(inputToken, "removemultipleinvitees") == 0){
    Error status = prepare_and_exec_remove_multiple_invitees(db, input, output);
    return error_to_json(status, output);

  }else if (strcmp(inputToken, "getmonthevents") == 0){
    Error status = prepare_and_exec_get_month_events(db, input, output);
    return error_to_json(status, output);

  }else{

    printf("Cause of ERROR --> %s | ERROR: Invalid command\n ", inputToken);
    return (Error) {INVALID_ARGUMENT,
                    "director.c/input_handler/ERROR: Invalid Command.\n"};
                    
  }
}

Error prepare_and_exec_create_user(sqlite3 **db, char *input, String *output){
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
  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB | JSON_C_TO_STRING_SPACED));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert add_friend result from JSON to string for output.\n");
  } 
  return status;
}

Error prepare_and_exec_delete_user(sqlite3 **db, char *input, String *output){
  char *userID = strtok(NULL, "|");
  if(userID == NULL) {
    fprintf(stderr, "ERROR: Missing userID for deleteuser command\n");
    return (Error) {INVALID_ARGUMENT, 
                    "director.c/input_handler/ERROR: Missing userID for deleteuser command.\n"};
  }
  Error status = delete_user(db, userID);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to delete user\n");
    return status;
  }
  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB | JSON_C_TO_STRING_SPACED));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert add_friend result from JSON to string for output.\n");
  }
  return status;
}

Error prepare_and_exec_login(sqlite3 **db, char *input, String *output){
  char *username = strtok(NULL, "|");
  char *password = strtok(NULL, "|");
  if(username == NULL || password == NULL) {
    fprintf(stderr, "ERROR: Missing parameters for login command\n");
    return (Error) {INVALID_ARGUMENT, 
                    "director.c/input_handler/ERROR: Missing parameters for login command.\n"};
  }
  String necessary_user_info = {.data = calloc(1, sizeof(char)), .length = 0, .capacity = 1}; // user_id, friends, invited
  Error status = login(db, username, password, &necessary_user_info);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to login user\n");
  }
  if(necessary_user_info.data == "Invalid credentials"){
    status = strcpy_dynamic(output, necessary_user_info.data);
    if(status.code != OK){
      fprintf(stderr, "ERROR: Failed to copy login results to output string.\n");
    }
    return status;
  }
  char *end;
  long user_id = strtol(strtok(necessary_user_info.data, "|"), &end, 10);

  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  json_object_object_add(json_output, "user_id", json_object_new_int64(user_id));
  json_object_object_add(json_output, "friends", json_object_new_string(strtok(NULL, "|")));
  json_object_object_add(json_output, "invited", json_object_new_string(strtok(NULL, "|")));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to stringify user info.\n");
  }
  return status;
}

Error prepare_and_exec_add_friend(sqlite3 **db, char *input, String *output){
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
    return status;
  }
  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert add_friend result from JSON to string for output.\n");
  }
  return status;
}

Error prepare_and_exec_remove_friend(sqlite3 **db, char *input, String *output){
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
    return status;
  }
  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert remove_friend result from JSON to string for output.\n");
  }
  return status;
}

Error prepare_and_exec_create_event(sqlite3 **db, char *input, String *output){
  char *user_id = strtok(NULL, "|");
  char *title = strtok(NULL, "|");
  char *start_date = strtok(NULL, "|");
  char *end_date = strtok(NULL, "|");
  char *start_time = strtok(NULL, "|");
  char *end_time = strtok(NULL, "|");
  char *location = strtok(NULL, "|");
  char *publicity_type = strtok(NULL, "|");
  char *invitees = strtok(NULL, "|");
  char *details = strtok(NULL, "|");
  if(user_id == NULL || title == NULL || start_date == NULL || end_date == NULL || start_time == NULL
    || end_time == NULL || location == NULL || publicity_type == NULL || invitees == NULL || details == NULL){
    fprintf(stderr, "ERROR: Invalid Arguments.\n");
    return (Error) {INVALID_ARGUMENT, "director.c/input_handler/ERROR: Missing parameters for createevent command.\n"};
  }
  Event new_event = {.user_id = user_id, .title = title, .start_date = start_date, .end_date = end_date, .start_time = start_time , 
                .end_time = end_time, .location = location, .publicity_type = publicity_type, .invitees = invitees, .details = details};
  String event_id = {.data = calloc(output->capacity, sizeof(char)), .capacity = output->capacity};
  if(event_id.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for event_id.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "director.c/input_handler/prepare_and_exec_create_event/ERROR: Failed to allocate memory for event_id.\n"};
  }
  Error status = create_event(db, new_event, &event_id);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to create event.\n");
    free(event_id.data);
    return status;
  }
  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  char *end;
  long event_id_int = strtol(event_id.data, &end, 10);
  free(event_id.data);
  json_object_object_add(json_output, "event_id", json_object_new_int64(event_id_int));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert create_event result from JSON to string for output.\n");
  }
  return status;
}

Error prepare_and_exec_delete_event(sqlite3 **db, char *input, String *output){
  char *event_id = strtok(NULL, "|");
  if(event_id == NULL){
    fprintf(stderr, "ERROR: Invalid Argument.\n");
    return (Error) {INVALID_ARGUMENT, "director.c/input_handler/ERROR: Missing parameters for deleteevent command.\n"};
  }
  Error status = delete_event(db, event_id);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to delete event.\n");
    return status;
  }
  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert delete_event result from JSON to string for output.\n");
  }
  return status;
}

Error prepare_and_exec_add_invitee(sqlite3 **db, char *input, String *output){
  char *event_id = strtok(NULL, "|");
  char *invitee = strtok(NULL, "|");
  printf("event_id: %s\n", event_id);
  printf("invitee: %s\n",invitee );
  if(event_id == NULL || invitee == NULL){
    fprintf(stderr, "ERROR: Invalid arguments.\n");
    return (Error) {INVALID_ARGUMENT, "director.c/input_handler/ERROR: Missing parameters for addinvitee command.\n"};
  }
  Error status = add_invitee(db, event_id, invitee);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to add invitee.\n");
    return status;
  }
  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert add_invitee result from JSON to string for output.\n");
  }
  return status;
}

Error prepare_and_exec_remove_invitee(sqlite3 **db, char *input, String *output){
  char *event_id = strtok(NULL, "|");
  char *invitee = strtok(NULL, "|");
  printf("event_id: %s\n", event_id);
  printf("invitee: %s\n",invitee );
  if(event_id == NULL || invitee == NULL){
    fprintf(stderr, "ERROR: Invalid arguments.\n");
    return (Error) {INVALID_ARGUMENT, "director.c/input_handler/ERROR: Missing parameters for removeinvitee command.\n"};
  }
  Error status = remove_invitee(db, event_id, invitee);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to remove invitee.\n");
    return status;
  }
  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB ));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert remove_invitee result from JSON to string for output.\n");
  }
  return status;
}

Error prepare_and_exec_remove_multiple_invitees(sqlite3 **db, char *input, String *output){
  char *event_id = strtok(NULL, "|");
  char *invitees = strtok(NULL, "|");
  if(event_id == NULL || invitees == NULL){
    fprintf(stderr, "ERROR: Invalid arguments.\n");
    return (Error) {INVALID_ARGUMENT, "director.c/input_handler/ERROR: Missing parameters for removemultipleinvitees command.\n"};
  }
  Error status = remove_multiple_invitees(db, event_id, invitees, false /*user_id option*/);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to remove invitees.\n");
    return status;
  }
  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert output to JSON string.\n");
    
  }
  return status;
}

Error prepare_and_exec_get_month_events(sqlite3 **db, char *input, String *output){
  char *user_id = strtok(NULL, "|");
  char *year = strtok(NULL, "|");
  char *month = strtok(NULL, "|");
  if(user_id == NULL || year == NULL || month == NULL){
    fprintf(stderr, "ERROR: Invalid input for getmonthevents.\n");
    return (Error) {INVALID_ARGUMENT,
                    "director.c/input_handler/ERROR: Invalid input for getmonthevents.\n"};
  }
  Event_Array month_events = {.data = calloc(1, sizeof(Event)), .length = 0, .table_capacity = 1};
  Error status = get_month_events(db, user_id, year, month, &month_events);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get month events.\n");
  }
  json_object *json_event_objects =  json_object_new_array();
  json_object_object_add(json_event_objects, "status", json_object_new_string("Success"));
  for(int i = 0; i < month_events.length; i ++){
    json_object *event;
    status = convert_event_to_json(event, month_events.data[i]);
    if(status.code != OK){
      fprintf(stderr,"ERROR: Failed to convert event to json object.\n");
      free(month_events.data);
      json_object_put(json_event_objects);
      return status;
    }
    json_object_array_add(json_event_objects, event);
  }
  free(month_events.data);
  char *events_stringified = json_object_to_json_string_ext(json_event_objects, JSON_C_TO_STRING_PRETTY_TAB);
  json_object_put(json_event_objects);
  status = strcpy_dynamic(output, events_stringified);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert result from JSON to string for output.\n");
  }
  return status;
}

Error error_to_json(Error status, String *output){
  if(status.code != OK){
    json_object *json_error_code = json_object_new_object();
    json_object_object_add(json_error_code, "status", json_object_new_string(status.message));
    strcpy_dynamic(output, json_object_to_json_string_ext(json_error_code, JSON_C_TO_STRING_PRETTY_TAB));
    json_object_put(json_error_code);
    return (Error) {OK, "Success"};
  }
  return status;
}

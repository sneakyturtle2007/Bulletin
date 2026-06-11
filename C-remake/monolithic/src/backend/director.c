//
// Created by luis on 6/4/25.
//

#include "director.h"

// Local function declarations 

Error prepare_and_exec_create_user(sqlite3 **db, String *output);

Error prepare_and_exec_delete_user(sqlite3 **db, String *output);

Error prepare_and_exec_login(sqlite3 **db, String *output);

Error prepare_and_exec_add_friends(sqlite3 **db, String *output);

Error prepare_and_exec_remove_friends(sqlite3 **db, String *output);

Error prepare_and_exec_create_event(sqlite3 **db, String *output);

Error prepare_and_exec_delete_event(sqlite3 **db, String *output);

Error prepare_and_exec_add_invitees(sqlite3 **db, String *output);

Error prepare_and_exec_remove_invitees(sqlite3 **db, String *output);

Error prepare_and_exec_get_month_events(sqlite3 **db,String *output);

Error prepare_and_exec_create_group(sqlite3** db, String *output);

Error prepare_and_exec_delete_group(sqlite3** db, String* output);

Error prepare_and_exec_add_users_to_group(sqlite3** db, String* output);

Error return_json_success(String* output);

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
    return prepare_and_exec_create_user(db, output);

  }else if(strcmp(inputToken, "deleteuser") == 0) {
    return prepare_and_exec_delete_user(db, output);

  }else if (strcmp(inputToken, "login") == 0) {
    return prepare_and_exec_login(db, output);

  }else if(strcmp(inputToken, "addfriends") == 0) {
    return prepare_and_exec_add_friends(db, output);

  }else if (strcmp(inputToken, "removefriends") == 0){
    return prepare_and_exec_remove_friends(db, output);

  }else if(strcmp(inputToken, "createevent") == 0){
    return prepare_and_exec_create_event(db, output);

  } else if(strcmp(inputToken, "deleteevent") == 0){
    return prepare_and_exec_delete_event(db, output);

  }else if(strcmp(inputToken, "addinvitees") == 0){
    return prepare_and_exec_add_invitees(db, output);

  }else if(strcmp(inputToken, "removeinvitees") == 0){
    return prepare_and_exec_remove_invitees(db, output);

  }else if (strcmp(inputToken, "getmonthevents") == 0){
    return prepare_and_exec_get_month_events(db, output);

  }else if(strcmp(inputToken, "creategroup") == 0){
    return prepare_and_exec_create_group(db, output);

  }else if(strcmp(inputToken, "deletegroup") == 0){
    return prepare_and_exec_delete_group(db, output);
  }else if(strcmp(inputToken, "adduserstogroup") == 0){
    return prepare_and_exec_add_users_to_group(db, output);
  
  }else{
    printf("Cause of ERROR --> %s | ERROR: Invalid command\n ", inputToken);
    return (Error) {INVALID_ARGUMENT,
                    "director.c/input_handler/ERROR: Invalid Command.\n"};
                    
  }
}

Error prepare_and_exec_create_user(sqlite3 **db, String *output){
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
  return return_json_success(output);
}

Error prepare_and_exec_delete_user(sqlite3 **db, String *output){
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
  return return_json_success(output);
}

Error prepare_and_exec_login(sqlite3 **db, String *output){
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
  json_object *json_output = json_object_new_object();

  if(strncmp(necessary_user_info.data, "Invalid credentials", necessary_user_info.length) == 0){
    json_object_object_add(json_output, "status", json_object_new_string(necessary_user_info.data));
    //printf(" DEBUG: necessary user info: %s\n", necessary_user_info.data); // DEBUG
    status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB));
    json_object_put(json_output);
    //free(necessary_user_info.data);
    if(status.code != OK){
      fprintf(stderr, "ERROR: Failed to copy login results to output string.\n");
    }
    return status;
  }
  char *end;
  long user_id = strtol(strtok(necessary_user_info.data, "|"), &end, 10);
  //printf("DEBUG: necessary user info: %s\n", necessary_user_info.data); // DEBUG
  //printf("DEBUG: user_id_long: %ld\n", user_id); // DEBUG
  
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  json_object_object_add(json_output, "user_id", json_object_new_int64(user_id));
  json_object_object_add(json_output, "friends", json_object_new_string(strtok(NULL, "|")));
  json_object_object_add(json_output, "invited", json_object_new_string(strtok(NULL, "|")));
  json_object_object_add(json_output, "groups", json_object_new_string(strtok(NULL, "|")));
  json_object_object_add(json_output, "friend_requests", json_object_new_string(strtok(NULL, "|")));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB));
  free(necessary_user_info.data);
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to stringify user info.\n");
  }
  return status;
}

Error prepare_and_exec_add_friends(sqlite3 **db, String *output){
  char *user_id = strtok(NULL, "|");
  char *friends_usernames = strtok(NULL, "|");
  if(user_id == NULL || friends_usernames == NULL){
    fprintf(stderr, "ERROR: Missing parameters for addfriend command.\n");
    return (Error) {INVALID_ARGUMENT,
                    "director.c/input_handler/ERROR: Missing parameters for addfriend command.\n"};
  }
  Error status;
  if(strchr(friends_usernames, ',') != NULL){
    status = add_multiple_friends(db, user_id, friends_usernames);
  }else{
    status = add_friend(db, user_id, friends_usernames);
  }
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to add friend.\n");
    return status;
  }
  return return_json_success(output);
}

Error prepare_and_exec_remove_friends(sqlite3 **db, String *output){
  char *user_id = strtok(NULL, "|");
  char *friends_usernames = strtok(NULL, "|");
  if(user_id == NULL || friends_usernames == NULL){
    fprintf(stderr, "ERROR: Missing parameters for removefriend command.\n");
    return (Error) {INVALID_ARGUMENT,
                    "director.c/input_handler/ERROR: Missing parameters for removefriend command.\n"};
  }
  Error status;
  if(strchr(friends_usernames, ',') != NULL){
    status = remove_multiple_friends(db, user_id, friends_usernames);
  }else{
    status = remove_friend(db, user_id, friends_usernames);
  }
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to remove friend(s).\n");
    return status;
  }
  return return_json_success(output);
}

Error prepare_and_exec_create_event(sqlite3 **db, String *output){
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
  //char *groups = strtok(NULL, "|");
  if(user_id == NULL || title == NULL || start_date == NULL || end_date == NULL || start_time == NULL
    || end_time == NULL || location == NULL || publicity_type == NULL || invitees == NULL || details == NULL){//|| groups == NULL 
    fprintf(stderr, "ERROR: Invalid Arguments.\n");
    return (Error) {INVALID_ARGUMENT, 
                    "director.c/prepare_and_exec_create_event/ERROR: Missing parameters for createevent command.\n"};
  }
  char *start_year = strtok(start_date, "-");
  char *start_month = strtok(NULL, "-");
  char *start_day = strtok(NULL, "-");
  char *end_year = strtok(end_date, "-");
  char *end_month = strtok(NULL, "-");
  char *end_day = strtok(NULL, "-");
  if(start_year == NULL || start_month == NULL || start_day == NULL || end_year == NULL || 
    end_month == NULL || end_day == NULL){
    fprintf(stderr, "ERROR: Invalid start or end date Arguments.\n");
    return (Error) {INVALID_ARGUMENT, 
                    "director.c/prepare_and_exec_create_event/ERROR: Invalid start or end date Arguments.\n"};
  }
  char *date_end;
  long start_year_l = strtol(start_year, &date_end, 10);
  long start_month_l = strtol(start_month, &date_end, 10);
  long start_day_l = strtol(start_day, &date_end, 10);
  long end_year_l = strtol(end_year, &date_end, 10);
  long end_month_l = strtol(end_month, &date_end, 10);
  long end_day_l = strtol(end_day, &date_end, 10);
  
  if(start_year_l == 0 || start_month_l == 0 || start_day_l == 0 || end_year_l == 0 || 
    end_month_l == 0 || end_day_l == 0){
    fprintf(stderr, "ERROR: Invalid event dates");
    return (Error) {INVALID_ARGUMENT,
                    "director.c/prepare_and_exec_create_event/ERROR: Invalid event dates.\n"};
  }
  String start_date_formatted = {.capacity = strlen(start_year) + strlen(start_month) + strlen(start_day) + 16}; // +16 for formatting
  start_date_formatted.data = calloc(start_date_formatted.capacity, sizeof(char));
  if(start_date_formatted.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for formatted event start date.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "director.c/prepare_and_exec_create_event/ERROR: Failed to allocate memory for formatted event start date.\n"};
  }
  snprintf(start_date_formatted.data, start_date_formatted.capacity, "%04ld-%02ld-%02ld", start_year_l, start_month_l, start_day_l);
  String end_date_formatted = {.capacity = strlen(end_year) + strlen(end_month) + strlen(end_day) + 16}; // +16 for formatting
  end_date_formatted.data = calloc(end_date_formatted.capacity, sizeof(char));
  if(end_date_formatted.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate mmeory for formatted event end date.\n");
    free(start_date_formatted.data);
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "director.c/prepare_and_exec_create_event/ERROR: Failed to allocate memory for formatted event end date.\n"};
  }
  snprintf(end_date_formatted.data, end_date_formatted.capacity, "%04ld-%02ld-%02ld", end_year_l, end_month_l, end_day_l);
  Event new_event = {.user_id = user_id, .title = title, .start_date = start_date_formatted.data, .end_date = end_date_formatted.data, .start_time = start_time , 
                .end_time = end_time, .location = location, .publicity_type = publicity_type, .invitees = invitees, .details = details};//, .groups = groups
  String event_id = {.data = calloc(output->capacity, sizeof(char)), .capacity = output->capacity};
  if(event_id.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for event_id.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "director.c/prepare_and_exec_create_event/prepare_and_exec_create_event/ERROR: Failed to allocate memory for event_id.\n"};
  }
  Error status = create_event(db, new_event, &event_id);
  free(start_date_formatted.data);
  free(end_date_formatted.data);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to create event.\n");
    free(event_id.data);
    return status;
  }
  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  char *event_id_end;
  long event_id_int = strtol(event_id.data, &event_id_end, 10);
  free(event_id.data);
  json_object_object_add(json_output, "event_id", json_object_new_int64(event_id_int));
  status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert create_event result from JSON to string for output.\n");
  }
  return status;
}

Error prepare_and_exec_delete_event(sqlite3 **db, String *output){
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
  return return_json_success(output);
}

Error prepare_and_exec_add_invitees(sqlite3 **db, String *output){
  char *event_id = strtok(NULL, "|");
  char *invitee = strtok(NULL, "|");
  //printf("event_id: %s\n", event_id); // DEBUG
  //printf("invitee: %s\n",invitee ); // DEBUG
  if(event_id == NULL || invitee == NULL){
    fprintf(stderr, "ERROR: Invalid arguments.\n");
    return (Error) {INVALID_ARGUMENT, "director.c/input_handler/ERROR: Missing parameters for addinvitee command.\n"};
  }
  Error status;
  if(strchr(invitee, ',') == NULL){
    status = add_invitee(db, event_id, invitee);
  }else{
    status = add_multiple_invitees(db, event_id, invitee);
  }
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to add invitee(s).\n");
    return status;
  }
  return return_json_success(output);
}

Error prepare_and_exec_remove_invitees(sqlite3 **db, String *output){
  char *event_id = strtok(NULL, "|");
  char *invitee_s = strtok(NULL, "|");
  //printf("event_id: %s\n", event_id); // DEBUG
  //printf("invitee: %s\n",invitee_s ); // DEBUG
  if(event_id == NULL || invitee_s == NULL){
    fprintf(stderr, "ERROR: Invalid arguments.\n");
    return (Error) {INVALID_ARGUMENT, "director.c/input_handler/ERROR: Missing parameters for removeinvitee command.\n"};
  }
  Error status;
  if(strchr(invitee_s, ',') != NULL){
    status = remove_multiple_invitees(db, event_id, invitee_s);
  }else{
    status = remove_invitee(db, event_id, invitee_s);
  }
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to remove invitee(s).\n");
    return status;
  }
  return return_json_success(output);
}

Error prepare_and_exec_get_month_events(sqlite3 **db, String *output){
  char *user_id = strtok(NULL, "|");
  char *year_s = strtok(NULL, "|");
  char *month_s = strtok(NULL, "|");
  if(user_id == NULL || year_s == NULL || month_s == NULL){
    fprintf(stderr, "ERROR: Invalid input for getmonthevents.\n");
    return (Error) {INVALID_ARGUMENT,
                    "director.c/input_handler/ERROR: Invalid input for getmonthevents.\n"};
  }
  char *end;
  long year = strtol(year_s, &end, 10);
  long month = strtol(month_s, &end, 10);
  if(year == 0 || month == 0){
    fprintf(stderr, "ERROR: Invalid input for getmonthevents.\n");
    return (Error) {INVALID_ARGUMENT,
                    "director.c/input_handler/ERROR: Invalid input for getmonthevents.\n"};
  }
  Event_Array month_events = {.data = calloc(1, sizeof(Event)), .length = 0, .capacity = 1};
  Error status = get_month_events(db, user_id, year, month, &month_events);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get month events.\n");
    free(month_events.data);
    return status;
  }
  json_object *json_event_objects =  json_object_new_array();
  json_object *status_json = json_object_new_object();
  if(month_events.length == 0){
    free(month_events.data);
    json_object_object_add(status_json, "status", json_object_new_string("No events found"));
    json_object_array_add(json_event_objects, status_json);
    status = strcpy_dynamic(output, json_object_to_json_string_ext(json_event_objects, JSON_C_TO_STRING_PRETTY_TAB));
    json_object_put(json_event_objects);
    if(status.code != OK){
      fprintf(stderr, "ERROR: Failed to copy stringified json events to output.\n");
    }
    return status;
  }else{
    json_object_object_add(status_json, "status", json_object_new_string("Success"));
    json_object_array_add(json_event_objects, status_json);
  }
  for(size_t i = 0; i < month_events.length; i ++){
    json_object *event = json_object_new_object();
    status = convert_event_to_json(event, month_events.data[i]);
    //printf("\n\n prepare_and_exec_get_month_events/eventid: %s\n\n", month_events.data[i].user_id); // DEBUG
    if(status.code != OK){
      fprintf(stderr,"ERROR: Failed to convert event to json object.\n");
      free(month_events.data);
      json_object_put(json_event_objects);
      return status;
    }
    json_object_array_add(json_event_objects, event);
  }
  free_event_array(&month_events);
  const char *events_stringified = json_object_to_json_string_ext(json_event_objects, JSON_C_TO_STRING_PRETTY_TAB);
  // printf("\n\n events stringified: %s\n\n", events_stringified); // DEBUG
  status = strcpy_dynamic(output, events_stringified);
  json_object_put(json_event_objects);
  //printf("\n\n output: %s\n\n", output->data); //DEBUG
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert result from JSON to string for output.\n");
  }
  return status;
}

Error prepare_and_exec_create_group(sqlite3** db, String *output){
  char* users = strtok(NULL, "|");
  char* public = strtok(NULL, "|");
  if(users == NULL || public == NULL){
    fprintf(stderr, "ERROR: Invalid input for create group.\n");
    return (Error) { INVALID_ARGUMENT,
                    "director.c/prepaer_and_exec_create_group/ERROR: Invalid input for create group.\n"};
  }
  Error status = create_group(db, users, public);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to create group.\n");
    return status;
  }
  return return_json_success(output);
}

Error prepare_and_exec_delete_group(sqlite3** db, String* output){
  char* group_id = strtok(NULL, "|");
  if(group_id == NULL){
    fprintf(stderr, "ERROR: Invalid input for delete group.\n");
    return (Error) {INVALID_ARGUMENT,
                    "director.c/prepare_and_exec_delete_group/ERROR: Invalid input for delet group.\n"};
  }
  Error status = delete_group(db, group_id);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to delete group.\n");
    return status;
  }
  return return_json_success(output);
}

Error prepare_and_exec_add_users_to_group(sqlite3** db, String* output){
  char* group_id = strtok(NULL, "|");
  char* users = strtok(NULL,"|");
  if(group_id == NULL || users == NULL){
    fprintf(stderr, "ERROR: Invalid input into add users to group.\n");
    return (Error) {INVALID_ARGUMENT,
                    "director.c/prepare_and_exec_add_user_to_group/ERROR: Invalid input into add users to group.\n"};
  }
  Error status = add_users_to_group(db, group_id, users);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to add users to group.\n");
    return status;
  }
  return return_json_success(output);
}

// Utility methods
Error error_to_json(Error status, String *output){
  json_object *json_error_code = json_object_new_object();
  json_object_object_add(json_error_code, "status", json_object_new_string(status.message));
  Error state = strcpy_dynamic(output, json_object_to_json_string_ext(json_error_code, JSON_C_TO_STRING_PRETTY_TAB));
  json_object_put(json_error_code);
  if(state.code != OK){
    fprintf(stderr, "ERROR: Failed to copy stringified json error code to output string.\n");
    return status;
  }
  return (Error) {OK, 
                  "Success"};
}

Error return_json_success(String* output){
  json_object *json_output = json_object_new_object();
  json_object_object_add(json_output, "status", json_object_new_string("Success"));
  Error status = strcpy_dynamic(output, json_object_to_json_string_ext(json_output, JSON_C_TO_STRING_PRETTY_TAB | JSON_C_TO_STRING_SPACED));
  json_object_put(json_output);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert add_friend result from JSON to string for output.\n");
  }
  return status;
}

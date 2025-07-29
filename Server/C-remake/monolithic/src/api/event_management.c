
#include "event_management.h"

// Function declarations (if needed)
Error convert_table_to_event(Event *event, Table_String *table);

Error create_event(sqlite3 **db, Event new_event, String *output){
  int values_length = strlen(new_event.user_id) + strlen(new_event.title) + strlen(new_event.start_date) + strlen(new_event.end_date) +
                      strlen(new_event.start_time) + strlen(new_event.end_time) + strlen(new_event.location) + strlen(new_event.publicity_type) + 
                      strlen(new_event.invitees) + strlen(new_event.details) + 128; // +128 for formatting 
  char *values = calloc(values_length, sizeof(char));
  if(values == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for condition.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/create_event/ERROR: Failed to allocate memory for values.\n"};
  }
  snprintf(values, values_length, "%s, '%s', '%s', '%s', %s, %s, '%s', '%s', '%s', '%s'", 
          new_event.user_id, new_event.title, new_event.start_date, new_event.end_date, new_event.start_time, new_event.end_time, 
          new_event.location, new_event.publicity_type, new_event.invitees, new_event.details);
  printf("%s\n", values);
  Error status = insert_into_table(db, "events", 
                                  "userid, title, start_date, end_date, startTime, endTime, location, publicityType, invitees, details", values);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to create event.\n");
    free(values);
    return status;
  }
  Table_String event = {.data = calloc(64, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 64};
  int condition_length = strlen(new_event.user_id) + 8; // +8 for formatting
  char *condition = malloc(condition_length);
  if(condition == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for condition.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/create_event/ERROR: Failed to allocate memory for condition.\n"};
  }
  snprintf(condition, condition_length, "userid=%s", new_event.user_id);
  printf("%s\n", condition);
  status = get_from_table(db, "MAX(eventid)", "events", condition, &event);
  free(condition);
  free(values);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get latest event.\n");
    return status;
  }
  status = strcpy_dynamic(output, event.data[0][0].data);
  free_table(&event);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to print event id to output string.\n");
  }
  return status;
}

Error delete_event(sqlite3 **db, char *event_id){
  Table_String event_info = {.data = calloc(1,sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 1};
  if(event_info.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for event info.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/delete_event/ERROR: Failed to allocate memory for event info.\n"};
  }
  String condition = {.capacity = strlen(event_id) + 16};// +16 for formatting
  condition.data = calloc(condition.capacity, sizeof(char));
  if(condition.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for query condition.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, "event_management.c/delete_event/ERROR: Failed to allocate memory for query condition.\n"};
  }
  snprintf(condition.data, condition.capacity, "eventid=%s", event_id);
  Error status = get_from_table(db, "*", "events", condition.data, &event_info);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to event info from events table.\n");
    free(condition.data);
    free_table(&event_info);
    return status;
  }
  char *invitees_list = event_info.data[0][8].data; 
  status = remove_multiple_invitees(db, event_id, invitees_list, true /*user_id option*/);
  free_table(&event_info);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to remove invitees.\n");
    free(condition.data);
    return status;
  }
  status = delete_from_table(db, "events", condition.data);
  free(condition.data);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to delete event from events table.\n");
  }
  return status;
}

Error add_invitee(sqlite3 **db, char *event_id, char *invitee){
  Table_String invitee_info = {.data = calloc(1, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 1};
  if(invitee_info.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for invitee info.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/add_invitee/ERROR: Failed to allocate memory for invitee info.\n"};
  }
  Error status = get_user_info(db, invitee, &invitee_info, false /*user_id option*/);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get invitee info.\n");
    free_table(&invitee_info);
    return status;
  }
  if(invitee_info.rows == 0){
    fprintf(stderr, "ERROR: Invitee %s doesn't exist.\n");
    free_table(&invitee_info);
    return (Error) {INVALID_ARGUMENT, 
                    "event_management.c/add_invitee/ERROR: Specified invitee doesn't exist.\n"};
  }
  String invitee_id = invitee_info.data[0][0]; // invitee_info.data[0][0] is the user_id of the invitee
  printf("invitee_id: %s\nevent_id: %s\n", invitee_id.data, event_id);
  status = update_users_invited_list(db, invitee_id.data, event_id, false /*remove option*/, true /*user_id option*/);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to update invitees invited list.\n");
    return status;
  }
  Table_String event_info = {.data = calloc(1, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 1};
  if(event_info.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for event_table.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, "event_management.c/add_invitee/ERROR: Failed to allocate memory for event table.\n"};
  }
  String condition = {.length = 0, .capacity = strlen(event_id) + 16}; // +16 for formatting
  condition.data = calloc(condition.capacity, sizeof(char));
  if(condition.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for query condition.\n");
    free_table(&event_info);
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/add_invitee/ERROR: Failed to allocate memory for query condition.\n"};
  }
  snprintf(condition.data, condition.capacity, "eventid=%s", event_id);
  status = get_from_table(db, "*", "events", condition.data, &event_info);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get event from table.\n");
    free(condition.data);
    free_table(&event_info);
    return status;
  }
  if(event_info.rows == 0){
    fprintf(stderr, "ERROR: Event %s doesn't exist.\n", event_id);
    free(condition.data);
    free_table(&event_info);
    return (Error) {INVALID_ARGUMENT, "event_management.c/add_invitee/ERROR: Specified event doesn't exist.\n"};
  }
  
  String invitees_list = event_info.data[0][8];
  String values = {.length = 0, .capacity = invitees_list.length + invitee_id.length + 1}; // +1 for comma 
  values.data = calloc(values.capacity, sizeof(char));
  if(values.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for invitee values.\n");
    free(condition.data);
    free(invitees_list.data);
    free(invitee_id.data);
    free_table(&event_info);
    free_table(&invitee_info);
    return (Error) {MEMORY_ALLOCATION_ERROR, "event_management.c/add_invitee/ERROR: Failed to allocate memory for invitee values.\n"};
  }
  if(strcmp(invitees_list.data, "NONE") == 0){
    snprintf(values.data, values.capacity, "%s", invitee_id.data);
    values.length += invitee_id.length;
  }else{
    snprintf(values.data, values.capacity, "%s,%s", invitees_list.data, invitee_id.data);
    values.length += invitees_list.length + invitee_id.length;
  }
  free_table(&invitee_info);
  free_table(&event_info);
  String variables_and_values = {.length = 0, .capacity = values.length + 16}; // +16 for formatting
  variables_and_values.data = calloc(variables_and_values.capacity, sizeof(char));
  if(variables_and_values.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for variables_and_values.\n");
    free(condition.data);
    free(values.data);
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/add_invitee/ERROR: Failed to allocate memory query variables_and_values.\n"};
  }
  snprintf(variables_and_values.data, variables_and_values.capacity, "invitees='%s'", values.data);
  free(values.data);
  status = update_table_info(db, "events", variables_and_values.data, condition.data);
  free(condition.data);
  free(variables_and_values.data);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to update invitees list for event in event table.\n");
    return status;
  }
  return status;
}

Error remove_invitee(sqlite3 **db, char *event_id, char *invitee){
  Table_String invitee_info = {.data = calloc(1, sizeof(String*)), .rows = 0, .cols = 0,  .table_capacity = 1};
  if(invitee_info.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for invitee info.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "event_management.c/remove_invitee/ERROR: Failed to allocate memory for invitee info.\n"};
  }
  Error status = get_user_info(db, invitee, &invitee_info, false /*user_id option*/);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get invitee info.\n");
    free_table(&invitee_info);
    return status;
  }
  String invitee_id = invitee_info.data[0][0];
  status = update_users_invited_list(db, invitee_id.data, event_id, true /*remove option*/, true /*if_user_id option*/);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to update invitees invited list.\n");
    free_table(&invitee_info);
    return status;
  }
  Table_String event_info = {.data = calloc(1, sizeof(String*)), .rows = 0 , .cols = 0, .table_capacity = 1};
  if(event_info.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for event_info table.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/remove_invitee/ERROR: Failed to allocate memory for event_info table.\n"};
  }
  String condition = {.data = calloc(strlen(event_id) + 16, sizeof(char)), .capacity = strlen(event_id) + 16}; // +16 for formatting
  if(condition.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for query condition.\n");
    free_table(&event_info);
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/remove_invitee/ERROR: Failed to allocate memory for query condition.\n"};
  }
  snprintf(condition.data, condition.capacity, "eventid='%s'", event_id);
  status = get_from_table(db, "*", "events", condition.data, &event_info);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get event info from events table.\n");
    free(condition.data);
    free_table(&event_info);
    return status;
  }
  if(event_info.rows == 0){
    fprintf(stderr, "ERROR: Event %s doesn't exist.\n");
    free(condition.data);
    free_table(&event_info);
    return (Error) {INVALID_ARGUMENT,
                    "event_management.c/remove_invitee/ERROR: Specified event doesn't exist.\n"};
  }
  String invitees_list = event_info.data[0][8];
  String values = {.length = 0, .capacity = invitees_list.length - invitee_id.length};
  bool invitees_list_empty = false;
  if(((int) values.capacity) < 2){ // Checks if values.capacity is less than 2 to account for any remaining commas and null-termination characters
    values.capacity = strlen("NONE") + 1;
    values.data = calloc(values.capacity, sizeof(char));
    invitees_list_empty = true;
  }else{
    printf("\nvalues.capacity: %d\n", values.capacity);
    printf("invitee_id length: %d\n\n", invitee_id.length);
    printf("invitee_id: %s\n", invitee_id.data);
    values.data = calloc(values.capacity, sizeof(char));
  }
  if(values.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for updated event values.\n");
    free(condition.data);
    free_table(&invitee_info);
    free_table(&event_info);
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "event_management.c/remove_invitee/ERROR: Failed to allocate memory for updated event values.\n"};
  }
  
  if(invitees_list_empty){
    snprintf(values.data, values.capacity, "%s", "NONE");
    values.length = strlen("NONE") + 1; // +1 for null termination character
  }else{
    String exclude_token = {.capacity = strlen(invitee) + 1};// +1 for comma
    exclude_token.data = calloc(exclude_token.capacity, sizeof(char)); 
    if(exclude_token.data == NULL){
      fprintf(stderr, "ERROR: Failed to allocate memory for exclude_token.\n");
      free(condition.data);
      free(values.data);
      free_table(&event_info);
      return (Error) {MEMORY_ALLOCATION_ERROR,
                      "event_management.c/remove_invitee/ERROR: Failed to allocate memory fo exclude_token.\n"};
    }
    if(strstr(invitees_list.data, invitee) == 0){
      snprintf(exclude_token.data, exclude_token.capacity, "%s,", invitee_id.data);
    }else{
      snprintf(exclude_token.data, exclude_token.capacity, ",%s", invitee_id.data);
    }
    status = strncpy_exclude(&values, invitees_list.data, exclude_token.data);
    free(exclude_token.data);
    if(status.code != OK){
      fprintf(stderr, "ERROR: Failed to cpy invitee list excluding invitee.\n");
      free(condition.data);
      free(values.data);
      free_table(&event_info);
    }
  }
  free_table(&invitee_info);
  free_table(&event_info);
  String variables_and_values = {.capacity = values.length + 16}; // +16 for formatting
  variables_and_values.data = calloc(variables_and_values.capacity, sizeof(char));
  if(variables_and_values.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for remove_invitee variables and values.\n");
    free(condition.data);
    free(values.data);
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "event_management.c/remove_invitee/ERRORP failed to allocate memory for remove_invitee variables and values.\n"};
  }
  snprintf(variables_and_values.data, variables_and_values.capacity, "invitees='%s'", values.data);
  free(values.data);
  status = update_table_info(db, "events", variables_and_values.data, condition.data);
  free(variables_and_values.data);
  free(condition.data);
  if(status.code != OK){
    fprintf(stderr,"ERROR: Failed to update event's invitees list.\n");
  }
  return status;
}

// Utility functions

Error remove_multiple_invitees(sqlite3 **db, char *event_id, char *invitees_list, bool user_id){
  if(strcmp(invitees_list, "NONE") == 0){
    return (Error) {OK, "Success"};
  }
  Error status;
  char *invitee = strtok(invitees_list, ",");
  printf("\n\nremove_multiple_invitees: event_id: %s\n", event_id);
  while(invitee != NULL && invitee[0] != '\0'){
    printf("invitee: %s\n", invitee);
    status = remove_invitee(db, event_id, invitee);
    if(status.code != OK){
      fprintf(stderr, "ERROR: Failed to delete invitee.\n");
      return status;
    }
    invitee = strtok(NULL, ",");
  }
  return status;
}

Error convert_table_to_list_of_events(Table_String table, Event_Array *event_array){
  if(table.cols < 9){
    printf("cols: %d\n", table.cols);
    fprintf(stderr, "ERROR: Invalid argument, not enough column in table.\n");
    return (Error) {INVALID_ARGUMENT,
                    "event_management.c/convert_table_to_event/ERROR: Invalid argument, not enough columns in table.\n"};
  }
  for(int col = 0; col < table.cols; col++){
    if(table.data[0][col].data == NULL){
      fprintf(stderr, "ERROR: Missing value in column.\n");
      return (Error) {INVALID_ARGUMENT, 
                      "event_management.c/convert_table_to_event/ERROR: Failed to allocate memory for event_variables.\n"};
    }
  } 
  if(event_array->table_capacity < table.rows){
    event_array->table_capacity = table.rows;
    Event *new_data = realloc(event_array->data, event_array->table_capacity);
    if(new_data == NULL){
      fprintf(stderr, "ERROR: Failed to reallocate memory for event_array.\n");
      return (Error) {MEMORY_ALLOCATION_ERROR,
                      "event_management.c/convert_table_to_list_of_event/ERROR: Failed to reallocate memory for event_array.\n"};
    }
    event_array->data = new_data;
  }
  for(int i = 0; i < table.rows; i ++){
    event_array->data[i].user_id = table.data[i][0].data;
    event_array->data[i].title = table.data[i][1].data;
    event_array->data[i].start_date = table.data[i][2].data;
    event_array->data[i].end_date = table.data[i][3].data;
    event_array->data[i].start_time = table.data[i][4].data;
    event_array->data[i].end_time = table.data[i][5].data;
    event_array->data[i].location = table.data[i][6].data;
    event_array->data[i].publicity_type = table.data[i][7].data;
    event_array->data[i].invitees = table.data[i][8].data;
    event_array->data[i].details = table.data[i][9].data;
    event_array->length ++;
  }
  return (Error) {OK, "Success"};
}
Error convert_event_to_json(json_object *result, Event event){
  char *end;
  long user_id = strtol(event.user_id, &end, 10);
  long start_time = strtol(event.start_time, &end, 10);
  long end_time = strtol(event.end_time, &end, 10);
  json_object_object_add(result, "user_id", json_object_new_int64(user_id));
  json_object_object_add(result, "title", json_object_new_string(event.title));
  json_object_object_add(result, "start_date", json_object_new_string(event.start_date));
  json_object_object_add(result, "end_date", json_object_new_string(event.end_date));
  json_object_object_add(result, "start_time", json_object_new_int64(start_time));
  json_object_object_add(result, "end_time", json_object_new_int64(end_time));
  json_object_object_add(result, "location", json_object_new_string(event.location));
  json_object_object_add(result, "publicity_type", json_object_new_string(event.publicity_type));
  json_object_object_add(result, "invitees", json_object_new_string(event.invitees));
  json_object_object_add(result, "details", json_object_new_string(event.details));
}
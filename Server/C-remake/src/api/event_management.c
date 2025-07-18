
#include "event_management.h"

// Function declarations (if needed)
Error remove_invitees(sqlite3 **db, char *event_id, char *invitees_list);
Error convert_table_to_event(Event *event, Table_String *table);

Error create_event(sqlite3 **db, Event new_event, String *output){
  int values_length = strlen(new_event.user_id) + strlen(new_event.title) + strlen(new_event.date) + strlen(new_event.start_time) 
                      + strlen(new_event.end_time) + strlen(new_event.location) + strlen(new_event.publicity_type) + 
                      strlen(new_event.invitees) + strlen(new_event.details) + 128; // +128 for formatting 
  char *values = calloc(values_length, sizeof(char));
  if(values == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for condition.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/create_event/ERROR: Failed to allocate memory for values.\n"};
  }
  snprintf(values, values_length, "%s, '%s', '%s', %s, %s, '%s', '%s', '%s', '%s'", 
          new_event.user_id, new_event.title, new_event.date, new_event.start_time, new_event.end_time, 
          new_event.location, new_event.publicity_type, new_event.invitees, new_event.details);
  printf("%s\n", values);
  Error status = insert_into_table(db, "events", "userid, title, date, startTime, endTime, location, publicityType, invitees, details", values);
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
  output->length = event.data[0][0].length + 1; // +1 for NULL-terminated character
  if(output->capacity < output->length){
    output->capacity = output->length;
    char *new_data = realloc(output->data, output->capacity);
    if(new_data == NULL){
      fprintf(stderr, "ERROR: Memory reallocation failed for output data.\n");
      free_table(&event);
      return (Error) {MEMORY_ALLOCATION_ERROR, 
                      "event_management.c/create_event/ERROR: Failed to reallocate memory for output data.\n"};
    }
  }
  snprintf(output->data, output->length, "%s", event.data[0][0].data);
  free_table(&event);
  output->length = strlen(output->data) + 1; // +1 for NULL-terminated character;
  output->data[output->length] = '\0';
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
  char *invitees_list = event_info.data[0][7].data;
  status = remove_invitees(db, event_id, invitees_list);
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
  Table_String event_info = {.data = calloc(1, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 1};
  if(event_info.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for event_table.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, "event_management.c/add_invitee/ERROR: Failed to allocate memory for event table.\n"};
  }
  String condition = {.length = 0, .capacity = strlen(event_id) + 16}; // +16 for formatting
  condition.data = calloc(condition.capacity, sizeof(char));
  if(condition.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for query condition.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/add_invitee/ERROR: Failed to allocate memory for query condition.\n"};
  }
  snprintf(condition.data, condition.capacity, "eventid=%s", event_id);
  Error status = get_from_table(db, "*", "events", condition.data, &event_info);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get event from table.\n");
    free(condition.data);
    free_table(&event_info);
    return status;
  }
  Table_String invitee_info = {.data = calloc(1, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 1};
  if(invitee_info.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for invitee info.\n");
    free(condition.data);
    free_table(&event_info);
    free_table(&invitee_info);
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/add_invitee/ERROR: Failed to allocate memory for invitee info.\n"};
  }
  status = get_user_info(db, invitee, &invitee_info, false /*user_id option*/);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get invitee info.\n");
    free(condition.data);
    free_table(&invitee_info);
    return status;
  }
  String invitee_id = invitee_info.data[0][0];
  status = update_users_invited_list(db, invitee_id.data, event_id, false /*remove option*/, true /*user_id option*/);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to update invitees invited list.\n");
    return status;
  }
  String invitees_list = event_info.data[0][7];
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
// Utility functions
Error remove_invitees(sqlite3 **db, char *event_id, char *invitees_list){
  if(strcmp(invitees_list, "NONE") == 0){
    return (Error) {OK, "Success"};
  }
  Error status;
  char *invitee = strtok(invitees_list, ",");
  while(invitee != NULL){
    status = update_users_invited_list(db, invitee, event_id, true /*remove option*/, true /*user_id option*/);
    if(status.code != OK){
      fprintf(stderr, "ERROR: Failed to update invitees invited list.\n");
      return status;
    }
    invitee = strtok(NULL, ",");
  }
  return status;
}
Error convert_table_to_event(Event *event, Table_String *table){
  if(table->cols < 9){
    printf("cols: %d\n", table->cols);
    fprintf(stderr, "ERROR: Invalid argument, not enough column in table.\n");
    return (Error) {INVALID_ARGUMENT,
                    "event_management.c/convert_table_to_event/ERROR: Invalid argument, not enough columns in table.\n"};
  }
  if(event == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for event_variables.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "event_management.c/convert_table_to_event/ERROR: Failed to allocate memory for event_variables.\n"};
  }
  for(int col = 0; col < table->cols; col++){
    if(table->data[0][col].data == NULL){
      fprintf(stderr, "ERROR: Missing value in column.\n");
      free(event);
      return (Error) {INVALID_ARGUMENT, 
                      "event_management.c/convert_table_to_event/ERROR: Failed to allocate memory for event_variables.\n"};
    }
  } 
  event->user_id = table->data[0][0].data;
  event->title = table->data[0][1].data;
  event->date = table->data[0][2].data;
  event->start_time = table->data[0][3].data;
  event->end_time = table->data[0][4].data;
  event->location = table->data[0][5].data;
  event->publicity_type = table->data[0][6].data;
  event->invitees = table->data[0][7].data;
  event->details = table->data[0][8].data;
  return (Error) {OK, "Success"};
}
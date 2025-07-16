
#include "event_management.h"

// Function declarations (if needed)
Error update_invitees_invited_list(char *invitee);
Error convert_table_to_event(Event *event, Table_String *table);
Error free_event(Event *event);

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
  int condition_length = strlen(event_id) + 16; // +16 for formatting
  char *condition = calloc(condition_length, sizeof(char));
  if(condition == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for query condition.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, "event_management.c/delete_event/ERROR: Failed to allocate memory for query condition.\n"};
  }
  snprintf(condition, condition_length, "eventid=%s", event_id);
  Error status = delete_from_table(db, "events", condition);
  free(condition);
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
  int condition_length = strlen(event_id) + 16; // +16 for formatting
  char *condition = calloc(condition_length, sizeof(char));
  if(condition == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for query condition.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/add_invitee/ERROR: Failed to allocate memory for query condition.\n"};
  }
  snprintf(condition, condition_length, "eventid=%s", event_id);
  Error status = get_from_table(db, "*", "events", condition, &event_info);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get event from table.\n");
    free(condition);
    free_table(&event_info);
    return status;
  }
  String invitees_list = event_info.data[0][7];
  Table_String invitee_info = {.data = calloc(1, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 1};
  if(invitee_info.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for invitee info.\n");
    free(condition);
    free_table(&event_info);
    free_table(&invitee_info);
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/add_invitee/ERROR: Failed to allocate memory for invitee info.\n"};
  }
  status = get_user_info(db, invitee, &invitee_info, false /*user_id*/);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get invitee info.\n");
    free(condition);
    free_table(&event_info);
    free_table(&invitee_info);
    return status;
  }
  char *invitee_id = invitee_info.data[0][0].data;
  int values_length = strlen(invitees_list.data) + strlen(invitee_id) + 1; // +1 for comma 
  char *values = calloc(values_length, sizeof(char));
  if(values == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for invitee values.\n");
    free(condition);
    free_table(&event_info);
    return (Error) {MEMORY_ALLOCATION_ERROR, "event_management.c/add_invitee/ERROR: Failed to allocate memory for invitee values.\n"};
  }
  if(strcmp(invitees_list.data, "NONE") == 0){
    snprintf(values, values_length, "%s", invitee_id);
  }else{
    snprintf(values, values_length, "%s,%s", invitees_list.data, invitee_id);
  }
  free_table(&event_info);
  free_table(&invitee_info);
  int variables_and_values_length = values_length + 8; // +8 for formatting
  char *variables_and_values = calloc(variables_and_values_length, sizeof(char));
  if(variables_and_values == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for variables_and_values.\n");
    free(condition);
    free(values);
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/add_invitee/ERROR: Failed to allocate memory query variables_and_values.\n"};
  }
  snprintf(variables_and_values, variables_and_values_length, "invitees='%s'", values);
  free(values);
  status = update_table_info(db, "events", variables_and_values, condition);
  free(condition);
  free(variables_and_values);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to update invitees list for event in event table.\n");
    return status;
  }
  return status;
}

// Utility functions
Error update_invitees_invited_list(char *invitee){

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
Error free_event(Event *event){
  free(event->user_id);
  free(event->title);
  free(event->date);
  free(event->start_time);
  free(event->end_time);
  free(event->location);
  free(event->publicity_type);
  free(event->invitees);
  free(event->details);
  return (Error) {OK, "Success"};
}

#include "event_management.h"

// Function declarations (if needed)
Error get_latest_user_event(sqlite3 **db, char *user_id, Table_String *event);

Error create_event(sqlite3 **db, Event new_event, String *output){
  int values_length = strlen(new_event.user_id) + strlen(new_event.title) + strlen(new_event.date) + strlen(new_event.start_time) 
                      + strlen(new_event.end_time) + strlen(new_event.location) + strlen(new_event.publicity_type) + 
                      strlen(new_event.invitees) + strlen(new_event.details) + 128; // +128 for formatting 
  char *values = calloc(values_length, sizeof(char));
  if(values == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for condition.\n");
    free(values);
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
  status = get_latest_user_event(db, new_event.user_id, &event);
  free(values);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get latest event.\n");
    return status;
  }
  sprintf(output->data, "%s", event.data[0][0].data);
  if(output->data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for output data.\n");
    free_table(&event);
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/create_event/ERROR: Failed to allocate memory for output data.\n"};
  }
  free_table(&event);
  output->length = strlen(output->data) + 1; // +1 for NULL-terminated character;
  output->data[output->length] = '\0';
  return status;
}

Error get_latest_user_event(sqlite3 **db, char *user_id, Table_String *event){
  int condition_length = strlen(user_id) + 8; // +8 for formatting
  char *condition = malloc(condition_length);
  if(condition == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for condition.\n");
    free(condition);
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/get_latest_user_event/ERROR: Failed to allocate memory for condition.\n"};
  }
  snprintf(condition, condition_length, "userid=%s", user_id);
  printf("%s\n", condition);
  Error status = get_from_table(db, "MAX(eventid)", "events", condition, event);
  free(condition);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get latest event from events table.\n");
  }
  return status;
}
#include "calendar_management.h"


Error get_month_events(sqlite3 **db, char *user_id, long year, long month, Event_Array *events){
  Table_String events_table = {.data = calloc(1, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 1};
  if(events_table.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for temp events table.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "calendar_management.c/get_month_events/ERROR: Failed to allocate memory for temp events table.\n"};
  }
  String condition = {.capacity = strlen(user_id) + 256};
  condition.data = calloc(condition.capacity, sizeof(char));
  if(condition.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for query condition.\n");
    free_table(&events_table);
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "calendar_management.c/get_month_events/ERROR: Failed to allocate memory for query condition.\n"};
  }
  snprintf(condition.data, condition.capacity, 
          "(userid = %s AND start_date <= '%04ld-%02ld-01' AND end_date >= '%04ld-%02ld-01') OR (userid = %s AND start_date BETWEEN '%04ld-%02ld-01' AND '%04ld-%02ld-31')",
          user_id, year, month, year, month, user_id, year, month, year, month);
  Error status = get_from_table(db, "*", "events", condition.data, &events_table);
  free(condition.data);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get month events from table.\n");
    free_table(&events_table);
    return status;
  }
  if(events_table.rows == 0){
    free_table(&events_table);
    return status;
  }
  status = convert_table_to_list_of_events(events_table, events);
  free_table(&events_table);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to convert table to ilst of events.\n");
  }
  return status;
}

Error get_month_schedule(sqlite3 **db, char *user_id, char *year, char *month, char *other_users, Event_Array *events){

}


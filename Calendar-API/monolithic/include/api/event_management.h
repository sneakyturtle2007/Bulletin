

#ifndef EVENT_MANAGEMENT_H
#define EVENT_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "json.h"
#include "error_handling.h"
#include "string_utils.h"
#include "table.h"
#include "database.h"
#include "user_management.h"

typedef struct{
  char *user_id; // 1
  char *title; // 2
  char *start_date; // 3
  char *end_date; //4
  char *start_time; // 5
  char *end_time; // 6
  char *location; // 7
  char *publicity_type; // 8
  char *invitees; // 9
  char *details; // 10
  char *groups;
} Event;

typedef struct{
  Event *data;
  size_t length;
  size_t capacity;
} Event_Array;

Error create_event(sqlite3 **db, Event new_event, String *output);

Error delete_event(sqlite3 **db, char *event_id);

Error add_invitee(sqlite3 **db, char *event_id, char *invitee);

Error add_multiple_invitees(sqlite3 **db, char *event_id, char *invitees_list);

Error remove_invitee(sqlite3 **db, char *event_id, char *invitee);

Error remove_multiple_invitees(sqlite3 **db, char *event_id, char *invitees_list);

Error convert_table_to_list_of_events(Table_String table, Event_Array *event_array);

Error convert_event_to_json(json_object *result, Event event);

#endif
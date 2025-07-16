

#ifndef EVENT_MANAGEMENT_H
#define EVENT_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "error_handling.h"
#include "string_utils.h"
#include "table.h"
#include "database.h"
#include "user_management.h"

typedef struct{
  char *user_id; // 1
  char *title; // 2
  char *date; // 3
  char *start_time; // 4
  char *end_time; // 5
  char *location; // 6
  char *publicity_type; // 7
  char *invitees; // 8
  char *details; // 9
} Event;

Error create_event(sqlite3 **db, Event new_event, String *output);

Error delete_event(sqlite3 **db, char *event_id);

Error add_invitee(sqlite3 **db, char *event_id, char *invitee);

#endif
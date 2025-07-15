

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

typedef struct{
  char *user_id;
  char *title;
  char *date;
  char *start_time;
  char *end_time;
  char *location;
  char *publicity_type;
  char *invitees;
  char *details;
} Event;

Error create_event(sqlite3 **db, Event new_event, String *output);

#endif
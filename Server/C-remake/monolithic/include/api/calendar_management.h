

#ifndef CALENDAR_MANAGEMENT_H
#define CALENDAR_MANAGEMENT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string_utils.h"
#include "table.h"
#include "error_handling.h"
#include "database.h"
#include "event_management.h"

Error get_month_events(sqlite3 **db, char *user_id, long year, long month, Event_Array *events);

#endif //CALENDAR_MANAGEMENT_H
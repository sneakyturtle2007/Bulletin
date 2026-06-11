#ifndef GROUP_MANAGEMENT_H
#define GROUP_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "string_utils.h"
#include "table.h"
#include "error_handling.h"
#include "database.h"

Error create_group(sqlite3** db, char* users, char* public);

Error delete_group(sqlite3** db, char* group_id);

Error add_users_to_group(sqlite3** db, char* group_id, char* users);

Error get_group_info(sqlite3** db, char* group_id, Table_String* result);

#endif
//
// Created by luis on 6/3/25.
//




#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "error_handling.h"
#include "database.h"
#include "string_utils.h"
#include "table.h"

Error create_user(sqlite3 **db, char *username, char *email, char *password);

Error delete_user(sqlite3 **db, char *userID);

Error update_user(sqlite3 **db, char *userID, char *username, char *email, char *password);

Error login(sqlite3 **db, char *username, char *password, String *output);

Error add_friend(sqlite3 **db, char *user_id, char *friend_username);

Error remove_friend(sqlite3 **db, char *user_id, char *friend_username);

Error get_user_info(sqlite3 **db, char *user_id_or_username, Table_String *result, bool user_id);

#endif //USER_MANAGEMENT_H

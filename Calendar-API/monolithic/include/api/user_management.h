//
// Created by luis on 6/3/25.
//




#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json.h>
#include "sqlite3.h"
#include "error_handling.h"
#include "table.h"
#include "database.h"
#include "string_utils.h"

#define USER_ID 0
#define USERNAME 1
#define USER_EMAIL 2

Error create_user(sqlite3 **db, char *username, char *email, char *password);

Error delete_user(sqlite3 **db, char *userID);

Error update_user(sqlite3 **db, char *userID, char *username, char *email, char *password);

Error login(sqlite3 **db, char *username, char *password, String *output);

Error add_friend(sqlite3 **db, char *user_id, char *friend_username);

Error remove_friend(sqlite3 **db, char *user_id, char *friend_username);

Error get_user_info(sqlite3 **db, char *user_id_or_username, Table_String *result, int user_id);

Error update_users_invited_list(sqlite3 **db, char *invitee, char *event_id, bool remove, int if_user_id);

#endif //USER_MANAGEMENT_H

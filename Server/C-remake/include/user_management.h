//
// Created by luis on 6/3/25.
//




#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include "database.h"

Error create_user(sqlite3 **db, char *username, char *email, char *password);

Error delete_user(sqlite3 **db, char *userID);

Error update_user(sqlite3 **db, char *userID, char *username, char *email, char *password);

Error login(sqlite3 **db, char *username, char *password, String *output);

Error add_friend(sqlite3 **db, char *user_id, char *friend_username);

Error remove_friend(sqlite3 **db, char *user_id, char *friend_username);

#endif //USER_MANAGEMENT_H

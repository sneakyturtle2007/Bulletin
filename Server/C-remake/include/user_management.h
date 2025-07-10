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

#endif //USER_MANAGEMENT_H

//
// Created by luis on 6/3/25.
//
#include "user_management.h"
#include "director.h"

int create_user(sqlite3 **db, char *username, char *password) {
    char command[256];
    sprintf(command, "INSERT INTO users (username, password) VALUES ('%s', '%s');", username, password);
    printf("%s\n", command);
    
    char *errmsg;
    int state = sqlite3_exec(*db, command, NULL, NULL, &errmsg);
    
    if(state != SQLITE_OK) {
        fprintf(stderr, "ERROR: Failed to create user %s\n%s", username, errmsg);
        sqlite3_free(errmsg);
        return 1; // Error code for user creation failure
    }
    
    return 0; // Success
}

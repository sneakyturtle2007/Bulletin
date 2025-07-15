//
// Created by luis on 6/3/25.
//

#include "user_management.h"

// Function declarations (if needed)
Error get_user_info(sqlite3 **db, char *user_id, Table_String *result);

// Function implementations
Error create_user(sqlite3 **db, char *username, char *email, char *password) {
    int condition_length = strlen(username) + 32; // +32 overhead for formatting
    char *condition = calloc(condition_length, sizeof(char));
    if(condition == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for condition string\n");
        free(condition);
        return (Error) {MEMORY_ALLOCATION_ERROR, 
                        "user_management.c/create_user/ERROR: Failed to allocate memory for condition string.\n"};
    }
    sprintf(condition, "username = '%s'", username);
    Table_String result = {.data = calloc(64, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 64};
    Error status = get_from_table(db, "*", "users", condition, &result);
    if(result.rows > 0){
        fprintf(stderr, "ERROR: User %s already exists\n", username);
        free_table(&result);
        free(condition);
        return (Error) {INVALID_ARGUMENT, "user_management.c/create_user/ERROR: User already exists.\n"};
    }
    
    int values_length = strlen(username) + strlen(email) + strlen(password) + 32; // +32 overhead for formatting
    char *values = calloc(values_length, sizeof(char));
    sprintf(values, "'%s', '%s', '%s'", username, email, password);
    if(values == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for values string\n");
        free(values);
        return (Error) {MEMORY_ALLOCATION_ERROR, 
                        "user_management.c/create_user/ERROR: Failed to allocate memory for values string.\n"};
    }
    status = insert_into_table(db, "users", "username, email, password", values);
    free(values);
    free(condition);
    free_table(&result);
    if(status.code != OK) {
        fprintf(stderr, "ERROR: Failed to create user %s\n", username); 
    }
    return status; 
}

Error delete_user(sqlite3 **db, char *user_id) {
    int condition_length = strlen(user_id) + 32; // +32 overhead for formatting
    char *condition = calloc(condition_length, sizeof(char));
    if(condition == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for condition string\n");
        free(condition);
        return (Error)  {MEMORY_ALLOCATION_ERROR, 
                        "user_management.c/delete_user/ERROR: Failed to allocate memory for condition string.\n"};
    }
    sprintf(condition, "id = %s", user_id);
    Table_String user_check = {.data = calloc(64, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 64};
    Error status = get_user_info(db, user_id, &user_check);
    if(status.code != OK){
        fprintf(stderr, "ERROR: Failed to get user info for %s\n%s", user_id, status.message);
        free(condition);
        free_table(&user_check);
        return status;
    }
    if(user_check.rows == 0) {
        fprintf(stderr, "ERROR: User %s does not exist\n", user_id);
        free(condition);
        free_table(&user_check);
        return (Error){INVALID_ARGUMENT, "user_management.c/delete_user/ERROR: User does not exist.\n"};
    }
    free_table(&user_check);
    status = delete_from_table(db, "users", condition);
    if(status.code != OK) {
        fprintf(stderr, "ERROR: Failed to delete user %s\n%s", user_id, status.message);
    }
    free(condition);
    return status;
}

Error update_user(sqlite3 **db, char *user_id, char *username, char *email, char *password) {
    int variables_and_values_length = strlen(username) + strlen(email) + strlen(password) + 64; // +64 overhead for formatting
    int condition_length = strlen(user_id) + 32; // +32 overhead for formatting
    char *variables_and_values = calloc(variables_and_values_length, sizeof(char));
    char *condition = calloc(condition_length, sizeof(char));
    sprintf(variables_and_values, "username = '%s', email = '%s', password = '%s'", username, email, password);
    sprintf(condition, "user_id = %s", user_id);
    if(variables_and_values == NULL || condition == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for command string\n");
        free(variables_and_values);
        free(condition);
        return (Error){ MEMORY_ALLOCATION_ERROR, 
                        "user_management.c/update_user/ERROR: Failed to allocate memory for variables_and_values or condition string.\n"};
    }
    Error status = update_table_info(db, "users", variables_and_values, condition);
    free(variables_and_values);
    free(condition);
    if(status.code != OK) {
        fprintf(stderr, "ERROR: Failed to update user %s\n%s", user_id, status.message);
    }
    return status;
}

Error login(sqlite3 **db, char *username, char *password, String *output){
    int condition_length = strlen(username) + 64; // +64 overhead for formatting
    char *condition = calloc(condition_length, sizeof(char));
    if(condition == NULL){
        fprintf(stderr, "ERROR: Memory allocation failed for condition string\n");
        free(condition);
        return (Error) {MEMORY_ALLOCATION_ERROR, 
                        "user_management.c/login/ERROR: Failed to allocate memory for condition string.\n"};
    }
    sprintf(condition, "username='%s'", username);
    Table_String result_table = {.data = calloc(64, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 64};
    Error status = get_from_table(db, "*", "users", condition, &result_table);
    if(status.code != OK) {
        fprintf(stderr, "ERROR: Failed to get user %s\n%s", username, status.message);
        free_table(&result_table);
        free(condition);
        return status;
    }
    // result_table.data[0][3] is the password field in the users table.
    if(strncmp(result_table.data[0][3].data, password, result_table.data[0][3].length) != OK) {
        char *result = "Invalid credentials";
        if(output->capacity < strlen(result) + 1){
            output->capacity = strlen(result) + 1;
            char *new_data = realloc(output->data, output->capacity);
            if(new_data == NULL) {
                fprintf(stderr, "ERROR: Memory allocation failed for output string\n");
                free_table(&result_table);
                free(condition);
                return (Error) {MEMORY_ALLOCATION_ERROR, 
                                "user_management.c/login/ERROR: Failed to allocate memory for output string.\n"};
            }
            output->data = new_data;
        }
        char *state = strncpy(output->data, result, strlen(result));
        if(state == NULL){
            fprintf(stderr, "ERROR: Failed to copy result to output string\n");
            free_table(&result_table);
            free(condition);
            return (Error) {STRING_ERROR, 
                            "user_management.c/login/ERROR: Failed to copy result to output string.\n"};
        }
        output->length = strlen(output->data) + 1; // +1 for null terminator
        output->data[output->length] = '\0';
        free_table(&result_table);
        free(condition);
        return status;
    }
    // result_table.data[0][0] is the user_id, result_table.data[0][4] is the email, and result_table.data[0][5] is the username.
    int result_length = result_table.data[0][0].length + result_table.data[0][4].length 
                        + result_table.data[0][5].length + 1; // +1 for null terminator
    char *result = calloc(result_length, sizeof(char));
    if(result == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for result string\n");
        free_table(&result_table);
        free(condition);
        return (Error) {MEMORY_ALLOCATION_ERROR, 
                        "user_management.c/login/ERROR: Failed to allocate memory for result string.\n"};
    }
    sprintf(result, "%s|%s|%s", result_table.data[0][0].data, result_table.data[0][4].data, result_table.data[0][5].data);
    if(output->capacity < result_length) {
        output->capacity = result_length;
        char *new_data = realloc(output->data, output->capacity);
        if(new_data == NULL) {
            fprintf(stderr, "ERROR: Memory allocation failed for output string\n");
            free_table(&result_table);
            free(condition);
            return (Error) {MEMORY_ALLOCATION_ERROR, 
                            "user_management.c/login/ERROR: Failed to allocate memory for output string.\n"};
        }
        output->data = new_data;
    }
    char *state = strncpy(output->data, result, strlen(result));
    if(state == NULL){
        fprintf(stderr, "ERROR: Failed to copy result to output string\n");
        free_table(&result_table);
        free(condition);
        return (Error) {STRING_ERROR, 
                        "user_management.c/login/ERROR: Failed to copy result to output string.\n"};
    }
    output->length = strlen(result);
    output->data[output->length] = '\0';
    free_table(&result_table);
    free(condition);
    free(result);
    return status;
}

Error add_friend(sqlite3 **db, char *user_id, char *friend_username){
    int condition_length = strlen(user_id) + 8; // +8 overhead for formatting
    char *condition = calloc(condition_length, sizeof(char));
    if(condition == NULL){
        fprintf(stderr, "ERROR: Memory allocation failed for condition.\n");
        free(condition);
        return (Error) {MEMORY_ALLOCATION_ERROR, 
                        "user_management.c/add_friend/ERROR: Failed to allocate memory for condition string.\n"};
    }
    sprintf(condition, "id=%s", user_id);
    Table_String user = {.data = calloc(64, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 64};
    Error status = get_user_info(db, user_id, &user);
    if(status.code != OK) {
        fprintf(stderr, "ERROR: Failed to get user info for %s\n%s", user_id, status.message);
        free(condition);
        free_table(&user);
        return status;
    }
    if(user.rows == 0){
        fprintf(stderr, "ERROR: User %s does not exist\n", user_id);
        free(condition);
        free_table(&user);
        return (Error){INVALID_ARGUMENT, "user_management.c/add_friend/ERROR: User does not exist.\n"};
    }
    String user_friends = user.data[0][4]; // user_info.data[0][4] is the friends list
    if(strcmp(user_friends.data, "NONE\0") == 0){
        char *state = strncpy(user_friends.data, "", 1);
        user_friends.length = 1;
        if(state == NULL){
            fprintf(stderr, "ERROR: Failed to copy empty string to user friends list\n");
            free(condition);
            free_table(&user);
            return (Error) {STRING_ERROR, 
                            "user_management.c/add_friend/ERROR: Failed to erase 'NONE' from friends list.\n"};
        }
    }
    int friends_list_length = user_friends.length + strlen(friend_username) + 2; // +2 for comma and null terminator
    char *friends_list = calloc(friends_list_length, sizeof(char));
    if(friends_list == NULL){
        fprintf(stderr, "ERROR: Memory allocation failed for friends list string.\n");
        free(condition);
        free(friends_list);
        free_table(&user);
        return (Error) {MEMORY_ALLOCATION_ERROR, 
                        "user_management.c/add_friend/ERROR: Failed to allocate memory for friends list string.\n"};
    }
    if(user_friends.length < 2){
        sprintf(friends_list, "%s", friend_username);
    }else{
        sprintf(friends_list, "%s,%s", user_friends.data, friend_username);
    }
    int variables_and_values_length = strlen(friends_list) + 32; // +32 overhead for formatting
    char *variables_and_values = calloc(variables_and_values_length, sizeof(char));
    if(variables_and_values == NULL){
        fprintf(stderr, "ERROR: Memory allocation failed for variables and values string.\n");
        free(condition);
        free(friends_list);
        free_table(&user);
        return (Error) {MEMORY_ALLOCATION_ERROR, 
                        "user_management.c/add_friend/ERROR: Failed to allocate memory for variables and values string.\n"};
    }
    snprintf(variables_and_values, variables_and_values_length, "friends='%s'", friends_list); 
    status = update_table_info(db, "users", variables_and_values, condition);
    free(condition);
    free(friends_list);
    free(variables_and_values);
    free_table(&user);
    if(status.code != OK){
        fprintf(stderr, "ERROR: Failed to update user %s with new friend %s\n%s", user_id, friend_username, status.message);
    }
    return status;
}

Error remove_friend(sqlite3 **db, char *user_id, char *friend_username){
	int condition_length = strlen(user_id) + 8; // +8 overhead for formatting
	char *condition = calloc(condition_length, sizeof(char));
	if(condition == NULL){
			fprintf(stderr, "ERROR: Memory allocation failed for condition.\n");
			free(condition);
			return (Error) {MEMORY_ALLOCATION_ERROR, 
											"user_management.c/remove_friend/ERROR: Failed to allocate memory for condition string.\n"};
	}
	sprintf(condition, "id=%s", user_id);
	Table_String user_info = {.data = calloc(64, sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 64};
	Error status = get_user_info(db, user_id, &user_info);
	if(status.code != OK){
			fprintf(stderr, "ERROR: Failed getting user info for %s\n%s", user_id, status.message);
			free(condition);
			free_table(&user_info);
			return status;
	}
	if(user_info.rows == 0){
			fprintf(stderr, "ERROR: User %s does not exist\n", user_id);
			free(condition);
			free_table(&user_info);
			return (Error){INVALID_ARGUMENT, "user_management.c/remove_friend/ERROR: User does not exist.\n"};
	}
	String user_friends = user_info.data[0][4]; // user_info.data[0][4] is the friends list column 
	int friends_list_length = user_friends.length - strlen(friend_username) - 1; // -1 for comma
	bool friends_list_empty = false;
	char *friends_list;
	if(friends_list_length < 2){
			friends_list_length += strlen("NONE") + 1; // +1 for NULL terminating character;
			friends_list = calloc(friends_list_length, sizeof(char));
			friends_list_empty = true;
	}else{
			friends_list = calloc(friends_list_length, sizeof(char));
	}
	if(friends_list == NULL){
			fprintf(stderr, "ERROR: Memory allocation failed for friends list string.\n");
			free(condition);
			free(friends_list);
			free_table(&user_info);
			return (Error) {MEMORY_ALLOCATION_ERROR, 
											"user_management.c/remove_friend/ERROR: Failed to allocate memory for friends list string.\n"};
	}
	if(friends_list_empty){
		snprintf(friends_list, friends_list_length, "%s", "NONE");
	}
	status = strncpy_exclude(&friends_list, user_friends.data, friends_list_length, friend_username);
	if(status.code != OK){
			fprintf(stderr, "ERROR: strncpy_exclude failed.\n");
			free(condition);
			free(friends_list);
			free_table(&user_info);
			return status;
	}
	int variables_and_values_length = friends_list_length + 32; // +32 overhead for formatting
	char *variables_and_values = calloc(variables_and_values_length, sizeof(char));
	if(variables_and_values == NULL){
			fprintf(stderr, "ERROR: Memory allocation failed for variables and values string.\n");
			free(condition);
			free(friends_list);
			free(variables_and_values);
			free_table(&user_info);
			return (Error) {MEMORY_ALLOCATION_ERROR, 
											"user_management.c/remove_friend/ERROR: Failed to allocate memory for variables and values string.\n"};
	}
	sprintf(variables_and_values, "friends='%s'", friends_list);
	status = update_table_info(db, "users", variables_and_values, condition);
	free(condition);
	free(friends_list);
	free(variables_and_values);
	free_table(&user_info);
	if(status.code != OK){
			fprintf(stderr, "ERROR: Failed to update user %s with new friend %s\n%s", user_id, friend_username, status.message);
	}
	return status;
}

// Internal functions to get user inforamation
Error get_user_info(sqlite3 **db, char *user_id, Table_String *result){
    int condition_length = strlen(user_id) + 64; // +64 overhead for formatting
    char *condition = calloc(condition_length, sizeof(char));
    if(condition == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for condition string\n");
        free(condition);
        return (Error) {MEMORY_ALLOCATION_ERROR, 
                        "user_management.c/get_user_info/ERROR: Failed to allocate memory for condition string.\n"};
    }
    sprintf(condition, "id = %s", user_id);
    Error status = get_from_table(db, "*", "users", condition, result);
    free(condition);
    if(status.code != OK) {
        fprintf(stderr, "ERROR: Failed to get user info for %s\n%s", user_id, status.message);
    }
    return status;
}

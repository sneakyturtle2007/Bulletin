//
// Created by luis on 6/3/25.
//

#include "database.h"

Error create_user(sqlite3 **db, char *username, char *email, char *password) {
    int condition_length = strlen(username) + 32; // +32 overhead for formatting
    char *condition = malloc(condition_length * sizeof(char));
    if(condition == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for condition string\n");
        free(condition);
        return (Error){MEMORY_ALLOCATION_ERROR, "user_management.c/create_user/ERROR: Failed to allocate memory for condition string.\n"};
    }
    sprintf(condition, "username = '%s'", username);
    Table_String result = {.data = malloc(64 * sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 64};
    Error status = get_from_table(db, "users", condition, &result);
    if(result.rows > 0){
        fprintf(stderr, "ERROR: User %s already exists\n", username);
        free_table(&result);
        free(condition);
        return (Error){INVALID_ARGUMENT, "user_management.c/create_user/ERROR: User already exists.\n"};
    }
    
    int values_length = strlen(username) + strlen(email) + strlen(password) + 32; // +32 overhead for formatting
    char *values = malloc(values_length * sizeof(char));
    sprintf(values, "'%s', '%s', '%s'", username, email, password);
    if(values == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for values string\n");
        free(values);
        return (Error){MEMORY_ALLOCATION_ERROR, "user_management.c/create_user/ERROR: Failed to allocate memory for values string.\n"};
    }
    status = insert_into_table(db, "users", "username, email, password", values);
    free(values);
    free_table(&result);
    if(status.code != OK) {
        fprintf(stderr, "ERROR: Failed to create user %s\n", username); 
    }
    return status; 
}

Error delete_user(sqlite3 **db, char *user_id) {
    int condition_length = strlen(user_id) + 32; // +32 overhead for formatting
    char *condition = malloc(condition_length * sizeof(char));
    sprintf(condition, "id = %s", user_id);
    printf("Condition: %s\n", condition);
    if(condition == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for condition string\n");
        free(condition);
        return (Error){MEMORY_ALLOCATION_ERROR, "user_management.c/delete_user/ERROR: Failed to allocate memory for condition string.\n"};
    }
    Error status = delete_from_table(db, "users", condition);
    if(status.code != OK) {
        fprintf(stderr, "ERROR: Failed to delete user %s\n%s", user_id, status.message);
    }
    free(condition);
    return status;
}

Error update_user(sqlite3 **db, char *user_id, char *username, char *email, char *password) {
    int variables_and_values_length = strlen(username) + strlen(email) + strlen(password) + 64; // +64 overhead for formatting
    int condition_length = strlen(user_id) + 32; // +32 overhead for formatting
    char *variables_and_values = malloc(variables_and_values_length * sizeof(char));
    char *condition = malloc(condition_length * sizeof(char));
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
    char *condition = malloc(condition_length * sizeof(char));
    if(condition == NULL){
        fprintf(stderr, "ERROR: Memory allocation failed for condition string\n");
        free(condition);
        return (Error){MEMORY_ALLOCATION_ERROR, "user_management.c/login/ERROR: Failed to allocate memory for condition string.\n"};
    }
    sprintf(condition, "username = '%s'", username);
    Table_String result_table = {.data = malloc(64 * sizeof(String*)), .rows = 0, .cols = 0, .table_capacity = 64};
    Error status = get_from_table(db, "users", condition, &result_table);
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
                return (Error){MEMORY_ALLOCATION_ERROR, "user_management.c/login/ERROR: Failed to allocate memory for output string.\n"};
            }
            output->data = new_data;
        }
        char *state = strncpy(output->data, result, strlen(result));
        if(state == NULL){
            fprintf(stderr, "ERROR: Failed to copy result to output string\n");
            free_table(&result_table);
            free(condition);
            return (Error){STRING_ERROR, "user_management.c/login/ERROR: Failed to copy result to output string.\n"};
        }
        output->length = strlen(output->data) + 1; // +1 for null terminator
        output->data[output->length] = '\0';
        free_table(&result_table);
        free(condition);
        return status;
    }
    // result_table.data[0][0] is the user_id, result_table.data[0][4] is the email, and result_table.data[0][5] is the username.
    int result_length = result_table.data[0][0].length + result_table.data[0][4].length + result_table.data[0][5].length + 1; // +1 for null terminator
    char *result = malloc(result_length * sizeof(char));
    if(result == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for result string\n");
        free_table(&result_table);
        free(condition);
        return (Error){MEMORY_ALLOCATION_ERROR, "user_management.c/login/ERROR: Failed to allocate memory for result string.\n"};
    }
    sprintf(result, "%s|%s|%s", result_table.data[0][0].data, result_table.data[0][4].data, result_table.data[0][5].data);
    if(output->capacity < result_length) {
        output->capacity = result_length;
        char *new_data = realloc(output->data, output->capacity);
        if(new_data == NULL) {
            fprintf(stderr, "ERROR: Memory allocation failed for output string\n");
            free_table(&result_table);
            free(condition);
            return (Error){MEMORY_ALLOCATION_ERROR, "user_management.c/login/ERROR: Failed to allocate memory for output string.\n"};
        }
        output->data = new_data;
    }
    char *state = strncpy(output->data, result, strlen(result));
    if(state == NULL){
        fprintf(stderr, "ERROR: Failed to copy result to output string\n");
        free_table(&result_table);
        free(condition);
        return (Error){STRING_ERROR, "user_management.c/login/ERROR: Failed to copy result to output string.\n"};
    }
    output->length = strlen(result);
    output->data[output->length] = '\0';
    free_table(&result_table);
    free(condition);
    free(result);
    return status;
}

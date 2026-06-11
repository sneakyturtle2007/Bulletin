#include "group_management.h"

Error create_group(sqlite3** db, char* users, char* public){
  String variables = {.capacity = 32, .length = 0};
  variables.data = calloc(variables.capacity, sizeof(char));
  if(variables.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for insertion variables.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "group_management.c/create_group/ERROR: Failed to allocate memory for insertion variables.\n"};
  }
  snprintf(variables.data, variables.capacity, "users, public");
  variables.length += strlen("users, public");
  String values = {.capacity = strlen(users) + 6, .length = 0};
  values.data = calloc(values.capacity, sizeof(char));
  if(values.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for insertion values.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "group_management.c/create_group/ERROR: Failed to allocate memory for insertion values.\n"};
  }
  snprintf(values.data, values.capacity, "'%s', %s", users, public);
  values.length += strlen(public);
  Error status = insert_into_table(db, "group", variables.data, values.data);
  free(variables.data);
  free(values.data);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to insert group in to groups table.\n");
  }
  return status;
}

Error delete_group(sqlite3** db, char* group_id){
  String condition = {.capacity = strlen(group_id) + 16, .length = 0};
  condition.data = calloc(condition.capacity, sizeof(char));
  if(condition.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for condition.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
      "group_management.c/delete_group/ERROR: Failed to allocation memory for condition.\n"};
  }
  snprintf(condition.data, condition.capacity, "group_id=%s", group_id);
  condition.length = strlen("group_id=") + strlen(group_id) + 1;
  Error status = delete_from_table(db, "groups", condition.data); 
  free(condition.data);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to delete from group from groups table.\n");
  }
  return status;
}

Error add_users_to_group(sqlite3** db, char* group_id, char* users){
  Table_String group_info = {.data = calloc(1, sizeof(String**)), .rows = 0, .cols = 0, .table_capacity = 1 };
  if(group_info.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory to group_info variable.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "group_management.c/add_user_to_group/ERROR: Failed to allocate memory to group_info variable.\n"};
  }
  Error status = get_group_info(db, group_id, &group_info);
  if(status.code != OK){
    free_table(&group_info);
    return status;
  }
  size_t group_user_str_len = strlen(group_info.data[0][1]->data);
  size_t user_str_len = strlen(users);
  String group_users = {.capacity = group_user_str_len + user_str_len + 1, .length = 0};
  group_users.data = calloc(group_users.capacity, sizeof(char));
  if(group_users.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for group_users variable.\n");
    free_table(&group_info);
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "group_management.c/add_user_to_group/ERROR: Failed to allocate memory for group_user variables.\n"};
  }  
  if(group_user_str_len > 1){
    snprintf(group_users.data, group_users.capacity, "%s,%s", group_info.data[0][1]->data, users);
  }else{
    snprintf(group_users.data, group_users.capacity, "%s", users);
  }
  group_users.length += group_user_str_len + user_str_len;
  free_table(&group_info);
  String variables_and_values = {.capacity = user_str_len + 1, .length = 0};
  variables_and_values.data = calloc(variables_and_values.capacity, sizeof(char));
  if(variables_and_values.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for query condition.\n");
    free_table(&group_info);
    free(group_users.data);
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "group_management.c/add_user_to_group/ERROR: Failed to allocate mmeroy for query condition.\n"};
  }
  snprintf(variables_and_values.data, variables_and_values.capacity, "users='%s'", group_users.data);
  free(group_users.data);
  String condition = {.capacity = strlen(group_id) + 1, .length = 0};
  condition.data = calloc(condition.capacity, sizeof(char));
  if(condition.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for query condition.\n");
    free(variables_and_values.data);
    return (Error) {MEMORY_ALLOCATION_ERROR,
                    "group_management.c/add_user_to_group/ERROR: Failed to allocate memory for query condition.\n"};
  }
  snprintf(condition.data, condition.capacity, "group_id='%s'", group_id);
  free(variables_and_values.data);
  status = update_table_info(db, "groups", variables_and_values.data, condition.data);
  free(condition.data);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to insert user into group.\n");
  }
  return status;
}

// Utility functions

// Also serves as regular service method.
Error get_group_info(sqlite3** db, char* group_id, Table_String* result){
  String condition = {.capacity = strlen(group_id) + 1};
  condition.data = calloc(condition.capacity, sizeof(char));
  condition.length = condition.capacity;
  if(condition.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory to condition.\n");
    return (Error){MEMORY_ALLOCATION_ERROR, 
                  "group_management.c/get_group_info/ERROR: Failed to allocate memory to group search condition.\n"};
  }
  snprintf(condition.data, condition.capacity, "group_id=%s",  group_id);
  Error status = get_from_table(db, "*", "groups", condition.data, result);
  free(condition.data);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get group info.\n");
  }
  return status;
}
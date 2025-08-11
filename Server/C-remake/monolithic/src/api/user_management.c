//
// Created by luis on 6/3/25.
//

#include "user_management.h"


// Function implementations
Error create_user(sqlite3 **db, char *username, char *email, char *password) {
	int condition_length = strlen(username) + 32; // +32 overhead for formatting
	char *condition = calloc(condition_length, sizeof(char));
	if(condition == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed for condition string\n");
		return (Error) {MEMORY_ALLOCATION_ERROR, 
										"user_management.c/create_user/ERROR: Failed to allocate memory for condition string.\n"};
	}
	sprintf(condition, "username = '%s'", username);
	Table_String user_check = {.data = calloc(64, sizeof(String**)), .rows = 0, .cols = 0, .table_capacity = 64};
	Error status = get_from_table(db, "*", "users", condition, &user_check);
	if(user_check.rows > 0){
		fprintf(stderr, "ERROR: User %s already exists\n", username);
		free_table(&user_check);
		free(condition);
		return (Error) {INVALID_ARGUMENT, "user_management.c/create_user/ERROR: User already exists.\n"};
	}
	free_table(&user_check);
	int values_length = strlen(username) + strlen(email) + strlen(password) + 32; // +32 overhead for formatting
	char *values = calloc(values_length, sizeof(char));
	sprintf(values, "'%s', '%s', '%s'", username, email, password);
	if(values == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed for values string\n");
		return (Error) {MEMORY_ALLOCATION_ERROR, 
										"user_management.c/create_user/ERROR: Failed to allocate memory for values string.\n"};
	}
	status = insert_into_table(db, "users", "username, email, password", values);
	free(values);
	free(condition);
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
		return (Error)  {MEMORY_ALLOCATION_ERROR, 
										"user_management.c/delete_user/ERROR: Failed to allocate memory for condition string.\n"};
	}
	sprintf(condition, "id = %s", user_id);
	Table_String user_check = {.data = calloc(64, sizeof(String**)), .rows = 0, .cols = 0, .table_capacity = 64};
	Error status = get_user_info(db, user_id, &user_check, USER_ID);
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
	
	if(variables_and_values == NULL || condition == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed for command string\n");
		free(variables_and_values);
		free(condition);
		return (Error){ MEMORY_ALLOCATION_ERROR, 
										"user_management.c/update_user/ERROR: Failed to allocate memory for variables_and_values or condition string.\n"};
	}
	sprintf(variables_and_values, "username = '%s', email = '%s', password = '%s'", username, email, password);
	sprintf(condition, "user_id = %s", user_id);
	Error status = update_table_info(db, "users", variables_and_values, condition);
	free(variables_and_values);
	free(condition);
	if(status.code != OK) {
		fprintf(stderr, "ERROR: Failed to update user %s\n%s", user_id, status.message);
	}
	return status;
}

Error login(sqlite3 **db, char *username, char *password, String *output){
	Table_String user_table = {.data = calloc(64, sizeof(String**)), .rows = 0, .cols = 0, .table_capacity = 64};
	if(user_table.data == NULL){
		fprintf(stderr, "ERROR: Failed to allocate memory for user_table.\n");
		free_table(&user_table);
		return (Error) {MEMORY_ALLOCATION_ERROR, "user_management.c/login/ERROR: Failed to allocate memory for user_table.\n"};
	}
	Error status;
	if(strchr(username, '@') != NULL){
		status = get_user_info(db, username, &user_table, USER_EMAIL);
	}else{
		status = get_user_info(db, username, &user_table, USERNAME);
	}
	if(status.code != OK){
		fprintf(stderr, "ERROR: Failed to get user info.\n");
		free_table(&user_table);
		return status;
	}
	// user_table.data[0][3] is the password field in the users table.
	if(strncmp(user_table.data[0][3]->data, password, user_table.data[0][3]->length) != 0) {
		char *result = "Invalid credentials";
		
		status = strcpy_dynamic(output, result);
		printf(" DEBUG: %s\n", output->data);
		free_table(&user_table);		
		if(status.code != OK){
			fprintf(stderr, "ERROR: Failed to copy result to output string\n");
		}
		return status;
	}
	/*
	user_table.data[0][0] is the user_id, user_table.data[0][4] is the friends, user_table.data[0][5] is the users list of events invited to,
	and user_table.data[0][6] is the groups the user is a part of.
	*/
	String necessary_user_info = {.capacity =  user_table.data[0][0]->length + user_table.data[0][4]->length 
											+ user_table.data[0][5]->length + user_table.data[0][6]->length + 1};// +1 for null terminator
	necessary_user_info.data = calloc(necessary_user_info.capacity, sizeof(char));
	if(necessary_user_info.data == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed for result string\n");
		free_table(&user_table);
		return (Error) {MEMORY_ALLOCATION_ERROR, 
										"user_management.c/login/ERROR: Failed to allocate memory for result string.\n"};
	}
	snprintf(necessary_user_info.data, necessary_user_info.capacity, "%s|%s|%s|%s", 
					user_table.data[0][0]->data, user_table.data[0][4]->data, user_table.data[0][5]->data, user_table.data[0][6]->data);
	status = strcpy_dynamic(output, necessary_user_info.data);
	free(necessary_user_info.data);
	free_table(&user_table);
	if(status.code != OK){
		fprintf(stderr, "ERROR: Failed to copy necessary user info to resulting output.\n");
	}
	return status;
}

Error add_friend(sqlite3 **db, char *user_id, char *friend_username){
	int condition_length = strlen(user_id) + 8; // +8 overhead for formatting
	char *condition = calloc(condition_length, sizeof(char));
	if(condition == NULL){
		fprintf(stderr, "ERROR: Memory allocation failed for condition.\n");
		return (Error) {MEMORY_ALLOCATION_ERROR, 
										"user_management.c/add_friend/ERROR: Failed to allocate memory for condition string.\n"};
	}
	sprintf(condition, "id=%s", user_id);
	Table_String user = {.data = calloc(64, sizeof(String**)), .rows = 0, .cols = 0, .table_capacity = 64};
	Error status = get_user_info(db, user_id, &user, USER_ID );
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
	String *user_friends = user.data[0][4]; // user_info.data[0][4] is the friends list
	if(strcmp(user_friends->data, "NONE\0") == 0){
		char *state = strncpy(user_friends->data, "", 1);
		user_friends->length = 1;
		if(state == NULL){
				fprintf(stderr, "ERROR: Failed to copy empty string to user friends list\n");
				free(condition);
				free_table(&user);
				return (Error) {STRING_ERROR, 
												"user_management.c/add_friend/ERROR: Failed to erase 'NONE' from friends list.\n"};
		}
	}
	int friends_list_length = user_friends->length + strlen(friend_username) + 2; // +2 for comma and null terminator
	char *friends_list = calloc(friends_list_length, sizeof(char));
	if(friends_list == NULL){
		fprintf(stderr, "ERROR: Memory allocation failed for friends list string.\n");
		free(condition);
		free_table(&user);
		return (Error) {MEMORY_ALLOCATION_ERROR, 
										"user_management.c/add_friend/ERROR: Failed to allocate memory for friends list string.\n"};
	}
	if(user_friends->length < 2){
		sprintf(friends_list, "%s", friend_username);
	}else{
		sprintf(friends_list, "%s,%s", user_friends->data, friend_username);
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
	Table_String user_info = {.data = calloc(64, sizeof(String**)), .rows = 0, .cols = 0, .table_capacity = 64};
	Error status = get_user_info(db, user_id, &user_info, USER_ID);
	if(status.code != OK){
		fprintf(stderr, "ERROR: Failed getting user info for %s\n%s", user_id, status.message);
		free_table(&user_info);
		return status;
	}
	if(user_info.rows == 0){
		fprintf(stderr, "ERROR: User %s does not exist\n", user_id);
		free_table(&user_info);
		return (Error){INVALID_ARGUMENT, "user_management.c/remove_friend/ERROR: User does not exist.\n"};
	}
	String *user_friends = user_info.data[0][4]; // user_info.data[0][4] is the friends list column 
	String friends_list = {.length = 0, .capacity = (user_friends->length - strlen(friend_username) - 1)};// -1 for comma
	bool friends_list_empty = false;
	if(friends_list.capacity < 2){
		friends_list.capacity = strlen("NONE") + 1; // +1 for NULL terminating character;
		friends_list.data = calloc(friends_list.capacity, sizeof(char));
		friends_list.length = friends_list.capacity;
		friends_list_empty = true;
	}else{
		friends_list.data = calloc(friends_list.capacity, sizeof(char));
	}
	if(friends_list.data == NULL){
		fprintf(stderr, "ERROR: Memory allocation failed for friends list string.\n");
		free(friends_list.data);
		return (Error) {MEMORY_ALLOCATION_ERROR, 
										"user_management.c/remove_friend/ERROR: Failed to allocate memory for friends list string.\n"};
	}
	if(friends_list_empty){
		snprintf(friends_list.data, friends_list.capacity, "%s", "NONE");
	}else{
		String temp_string = { .data = calloc(strlen(friend_username) + 2, sizeof(char)), .length = 0, .capacity = strlen(friend_username) + 2}; // +2 for comma and null termination character
		char *event_id_position = strstr(user_friends->data, friend_username);
		if(event_id_position - user_friends->data == 0 ){
			if(strlen(friend_username) +1 < user_friends->length){
				snprintf(temp_string.data, temp_string.capacity, "%s,", friend_username);
			}else{
				snprintf(temp_string.data, temp_string.capacity, "%s", friend_username);
			}
			
		}else{
			snprintf(temp_string.data, temp_string.capacity, ",%s", friend_username);
		}
		status = strncpy_exclude(&friends_list, user_friends->data, friend_username);
	}
	printf("Friend_list\n data: %s\n length: %d\n capacity: %d\n", friends_list.data,friends_list.length, friends_list.capacity); // DEBUG
	free_table(&user_info); // free'd here because otherwise it would free the memory user_friends.data pointer points to
	if(status.code != OK){
		fprintf(stderr, "ERROR: strncpy_exclude failed.\n");
		free(friends_list.data);
		return status;
	}
	String variables_and_values = {.capacity = friends_list.length + 32};// +32 overhead for formatting
	variables_and_values.data = calloc(variables_and_values.capacity, sizeof(char));
	if(variables_and_values.data == NULL){
		fprintf(stderr, "ERROR: Memory allocation failed for variables and values string.\n");
		free(friends_list.data);
		free(variables_and_values.data);
		return (Error) {MEMORY_ALLOCATION_ERROR, 
										"user_management.c/remove_friend/ERROR: Failed to allocate memory for variables and values string.\n"};
	}
	snprintf(variables_and_values.data, variables_and_values.capacity, "friends='%s'", friends_list.data);
	free(friends_list.data);
	int condition_length = strlen(user_id) + 8; // +8 overhead for formatting
	char *condition = calloc(condition_length, sizeof(char));
	if(condition == NULL){
		fprintf(stderr, "ERROR: Memory allocation failed for condition.\n");
		free(condition);
		return (Error) {MEMORY_ALLOCATION_ERROR, 
										"user_management.c/remove_friend/ERROR: Failed to allocate memory for condition string.\n"};
	}
	snprintf(condition, condition_length, "id=%s", user_id);
	status = update_table_info(db, "users", variables_and_values.data, condition);
	free(condition);
	free(variables_and_values.data);
	if(status.code != OK){
		fprintf(stderr, "ERROR: Failed to update user %s with new friend %s\n%s", user_id, friend_username, status.message);
	}
	return status;
}

// Internal functions to get user inforamation

Error get_user_info(sqlite3 **db, char *user_identifying_info, Table_String *result, int id_type){
	int condition_length = strlen(user_identifying_info) + 64; // +64 overhead for formatting
	char *condition = calloc(condition_length, sizeof(char));
	if(condition == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed for condition string\n");
		free(condition);
		return (Error) {MEMORY_ALLOCATION_ERROR, 
										"user_management.c/get_user_info/ERROR: Failed to allocate memory for condition string.\n"};
	}
	switch(id_type){
		case USER_ID:
			snprintf(condition, condition_length, "id=%s", user_identifying_info);
			break;
		case USERNAME:
			snprintf(condition, condition_length, "username='%s'", user_identifying_info);
			break;
		case USER_EMAIL:
			snprintf(condition, condition_length, "email='%s'", user_identifying_info);
			break;
	}
	
	Error status = get_from_table(db, "*", "users", condition, result);
	free(condition);
	if(status.code != OK) {
		fprintf(stderr, "ERROR: Failed to get user info for %s\n%s", user_identifying_info, status.message);
	}
	return status;
}

// Utility functions

Error update_users_invited_list(sqlite3 **db, char *invitee, char *event_id, bool remove, int id_type){
  Table_String invitee_info = {.data = calloc(1, sizeof(String**)), .rows = 0, .cols = 0, .table_capacity = 1};
  if(invitee_info.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for invitee info.\n");
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/add_invitee/ERROR: Failed to allocate memory for invitee info.\n"};
  }
  Error status = get_user_info(db, invitee, &invitee_info, id_type /*user_id, email, or username option*/);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to get invitee info.\n");
    free_table(&invitee_info);
    return status;
  }
  //invitee_info->data[0] is (user_id, username, email, password, friends, invited)
  String invited_list = {.length = 0, .capacity = (invitee_info.data[0][5]->length + strlen(event_id) + 2)}; // +2 for comma and NULL termination character
  invited_list.data = calloc(invited_list.capacity, sizeof(char));
  if(invited_list.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for invited_list.\n");
		free_table(&invitee_info);
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/update_invitees_invited_list/ERROR: Failed to allocate memory for invited_list.\n"};
  }
	snprintf(invited_list.data, invited_list.capacity, "%s", invitee_info.data[0][5]->data);
  if(strcmp(invited_list.data, "NONE") == 0){
    if(remove){
			free(invited_list.data);
			free_table(&invitee_info);
      return (Error) {OK, "Success"};
    }else{
      snprintf(invited_list.data, invited_list.capacity, "%s", event_id);
    }
  }else{
    if(!remove){
      snprintf(invited_list.data, invited_list.capacity, "%s,%s", invited_list.data, event_id);
    }else{
			String temp_string = { .data = calloc(strlen(event_id) + 2, sizeof(char)), .length = 0, .capacity = strlen(event_id) + 2}; // +2 for comma and null termination character
			char *event_id_position = strstr(invited_list.data, event_id);
			if(event_id_position - invited_list.data == 0 ){
				if(strlen(event_id) +1 < invited_list.length){
					snprintf(temp_string.data, temp_string.capacity, "%s,", event_id);
				}else{
					snprintf(temp_string.data, temp_string.capacity, "%s", event_id);
				}
				
			}else{
				snprintf(temp_string.data, temp_string.capacity, ",%s", event_id);
			}
			String temp_invited_list = {.data = calloc(invited_list.capacity + 1, sizeof(char)), .length = 0, .capacity = invited_list.capacity}; // +1 for null-termination character
			if(temp_invited_list.data == NULL){
				fprintf(stderr, "ERROR: Failed to allocate memory for temp invited list.\n");
				free(temp_string.data);
				free(invited_list.data);
				free_table(&invitee_info);
				return (Error) {MEMORY_ALLOCATION_ERROR,
												"user_management.c/update_users_invited_list/ERROR: Failed to allocate memory for temp invited list.\n"};
			}
			printf("update_users_invited_list\nevent_id: %s\n", event_id);
			printf("invited_list: %s\nTemp_string: %s\n", invited_list.data, temp_string.data);
      status = strncpy_exclude(&temp_invited_list, invited_list.data, temp_string.data);
			snprintf(invited_list.data, invited_list.capacity, "%s", temp_invited_list.data);
			free(temp_invited_list.data);
			free(temp_string.data);
      if(status.code != OK){
        fprintf(stderr, "ERROR: Failed to remove event_id from invitees invited list.\n");
        free(invited_list.data);
				free_table(&invitee_info);
        return status;
      }
			if(strlen(invited_list.data) == 0){
				char *state = strncat(invited_list.data, "NONE", 5);
			}
    }
  }
  invited_list.length = invited_list.capacity;
  String variables_and_values = {.capacity = invited_list.length + 16};// +16 for formatting
  variables_and_values.data = calloc(variables_and_values.capacity, sizeof(char));
  if(variables_and_values.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for variables_and_values.\n");
    free(invited_list.data);
		free_table(&invitee_info);
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/update_invitees_invited_list/ERROR: Failed to allocate memory for variables_and_values.\n"};
  }
  snprintf(variables_and_values.data, variables_and_values.capacity, "invited='%s'", invited_list.data);
  free(invited_list.data);
  String condition = {.capacity = invitee_info.data[0][0]->length + 16};// +16 for formatting
  condition.data = calloc(condition.capacity, sizeof(char));
  if(condition.data == NULL){
    fprintf(stderr, "ERROR: Failed to allocate memory for update condition.\n");
		free_table(&invitee_info);
    return (Error) {MEMORY_ALLOCATION_ERROR, 
                    "event_management.c/update_invitees_invited_list/ERROR: Failed to allocate memory for update condition.\n"};
  }
  snprintf(condition.data, condition.capacity, "id=%s", invitee_info.data[0][0]->data);
  free_table(&invitee_info);
  status = update_table_info(db, "users", variables_and_values.data, condition.data); 
  free(variables_and_values.data);
  free(condition.data);
  if(status.code != OK){
    fprintf(stderr, "ERROR: Failed to update invitees invited list.\n");
  }
  return status;
}
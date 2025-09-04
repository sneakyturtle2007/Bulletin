
#include "table.h"

Error free_table(Table_String *table){
	if(table == NULL || table->data == NULL) {
		return (Error) {OK, "Nothing to free\n"};
	}
	for(int i = 0; i < table->rows; i++){
		if(table->data[i] != NULL) {
			for(int j = 0; j < table->cols; j++){
				free(table->data[i][j]->data); // Free string data for column j at row i
				free(table->data[i][j]); // Free String struct for column j at row i
			}
			free(table->data[i]); // Free row of String structs
		}
	}
	free(table->data);
	table->rows = 0;
	table->cols = 0;
	table->table_capacity = 0;
	return (Error) {OK, "Memory freed\n"};
}

int convert_to_string_table(void *data, int numCols, char **colValues, char **colNames){
	Table_String *table = (Table_String *)data;
	printf("DEBUG: About to enter function\n"); // DEBUG
	printf("DEBUG: table->rows=%zu, table->table_capacity=%zu\n", table->rows, table->table_capacity); // DEBUG
	printf("DEBUG: numCols=%d\n", numCols); // DEBUG
	if(table->rows >= table->table_capacity) {
		table->table_capacity *= 2;
		table->data = realloc(table->data, table->table_capacity * sizeof(String**));
		if(table->data == NULL) {
			fprintf(stderr, "Memory allocation failed\n");
			return 1; // Memory allocation error
		}
	}
	table->data[table->rows] = calloc(numCols, sizeof(String*));
	if(!table->data[table->rows]) {
		fprintf(stderr, "Memory allocation failed\n");
		return 1;
	}
	table->cols = numCols;
	for(int i = 0; i < table->cols; i++){
		char *column_value;
		if(colValues[i] == NULL){
			column_value = "NONE";
		}else{
			column_value = colValues[i];
		}
		printf("DEBUG: colValues[%i] adjusted for possible null values: %s\n", i, column_value); // DEBUG
		printf("DEBUG: colValues[%i] strlen: %i\n", i, strlen(column_value)); // DEBUG
		String *temp_string = malloc(sizeof(String));
		if(temp_string == NULL) {
			fprintf(stderr, "Memory allocation failed for String struct\n");
			return 1; // Memory allocation error
		}
		temp_string->length = strlen(column_value) + 1; // +1 for null termination character
		temp_string->capacity = strlen(column_value) + 1; // +1 for null termination character
		temp_string->data = calloc(temp_string->capacity, sizeof(char));
		if(temp_string->data == NULL) {
			fprintf(stderr, "Memory allocation failed for String data\n");
			free(temp_string);
			return 1; // Memory allocation error
		}
		char *status = strncpy(temp_string->data, column_value, temp_string->length);
		if(status == NULL){
			fprintf(stderr, "ERROR: strncpy failed (function convert_to_string_table)\n");
			return 1; 
		}
		//status = memcpy(&table->data[table->rows][i], &temp_string, sizeof(temp_string));
		table->data[table->rows][i] = temp_string; // Assign the String struct to the table
	} 
	table->rows++;
	
	return 0; // Success
}

Error convert_table_to_string(Table_String *table, String *result){
  char *state;
  for(int i = 0; i < table->rows; i++){
    for(int k = 0; k < table->cols; k++){
      if(result->capacity < result->length + strlen(table->data[i][k]->data) + 2){
        result->capacity *= 2;
        char *new_data = realloc(result->data, result->capacity);
        if(new_data == NULL){
          fprintf(stderr, "ERROR: Failed to reallocate more memory for string data\n");
          return (Error) {MEMORY_ALLOCATION_ERROR, 
                          "director.c/convert_table_to_string/ERROR: Failed to reallocate more memory for result string.\n"};
        }
        result->data = new_data;
      }
      state = strncat(result->data, table->data[i][k]->data, strlen(table->data[i][k]->data));
      if(state == NULL){
        fprintf(stderr, "ERROR: failed to concatenate table data to resulting string\n");
        return (Error) {STRING_ERROR, 
                        "director.c/convert_table_to_string/ERROR: Failed to concatenate table data to resulting string.\n"};
      }
      state = strncat(result->data, "|", 2);
      if(state == NULL){
        fprintf(stderr, "ERROR: failed to concatenate separating symbol to resulting string\n");
        return (Error) {STRING_ERROR, 
                        "`director.c/convert_table_to_string/ERROR: Failed to concatenate column separator symbol.\n"};
      }
      result->length += (strlen(table->data[i][k]->data) + 1);
    }
    state = strncat(result->data, "\n", 2);
    result->length += 1; // Increment length for the new line character
    if(state == NULL){
      fprintf(stderr, "ERROR: Failed to convert table to string (failed to concatenate new line character onto end of row %d)\n", i);
      return (Error) {STRING_ERROR,
                      "director.c/convert_table_to_string/ERROR: Failed to concatenate new line character onto end of row.\n"};
    }
  }
  return (Error) {OK, "Success.\n"};
}


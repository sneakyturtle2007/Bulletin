
#include "table.h"

Error free_table(Table_String *table){
	if(table == NULL || table->data == NULL) {
		return (Error) {OK, "Nothing to free\n"};
	}
	for(int i = 0; i < table->rows; i++){
		if(table->data[i] != NULL) {
			for(int j = 0; j < table->cols; j++){
				free(table->data[i][j].data); // Free string data
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
	for(int i = 0; i < numCols; i++){
		if(colValues[i] == NULL){
			colValues[i] = "NONE"; // Handle NULL values
		}
	}
	table->data[table->rows] = calloc(numCols, sizeof(String));
	if(!table->data[table->rows]) {
		fprintf(stderr, "Memory allocation failed\n");
		return 1;
	}
	table->cols = numCols;
	for(int i = 0; i < numCols; i++){
		String temp_string = {.data = calloc(strlen(colValues[i]) + 1, sizeof(char)), .length = strlen(colValues[i]) + 1,
													.capacity = strlen(colValues[i]) + 1};
		void *status = strncpy(temp_string.data, colValues[i], temp_string.length);
		if(status == NULL){
			fprintf(stderr, "ERROR: strncpy failed (function convert_to_string_table)\n");
			free(temp_string.data); 
			return 1; 
		}
		//status = memcpy(&table->data[table->rows][i], &temp_string, sizeof(temp_string));
		table->data[table->rows][i] = temp_string;
	} 
	table->rows++;
	if(table->rows >= table->table_capacity) {
		table->table_capacity *= 2;
		table->data = realloc(table->data, table->table_capacity * sizeof(String*));
		if(!table->data) {
			fprintf(stderr, "Memory allocation failed\n");
			return 1; // Memory allocation error
		}
	}
	return 0; // Success
}

Error convert_table_to_string(Table_String *table, String *result){
  char *state;
  for(int i = 0; i < table->rows; i++){
    for(int k = 0; k < table->cols; k++){
      if(result->capacity < result->length + strlen(table->data[i][k].data) + 2){
        result->capacity *= 2;
        char *new_data = realloc(result->data, result->capacity);
        if(new_data == NULL){
          fprintf(stderr, "ERROR: Failed to reallocate more memory for string data\n");
          return (Error) {MEMORY_ALLOCATION_ERROR, 
                          "director.c/convert_table_to_string/ERROR: Failed to reallocate more memory for result string.\n"};
        }
        result->data = new_data;
      }
      state = strncat(result->data, table->data[i][k].data, strlen(table->data[i][k].data));
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
      result->length += (strlen(table->data[i][k].data) + 1);
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


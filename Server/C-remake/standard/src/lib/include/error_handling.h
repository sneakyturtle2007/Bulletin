

#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H 

typedef enum {
    OK = 0,
    MEMORY_ALLOCATION_ERROR = 1,
    DATABASE_ERROR = 2,
    INVALID_ARGUMENT = 3,
    STRING_ERROR = 4,
    TABLE_ERROR = 5
} ERROR_CODE;

typedef struct{
    ERROR_CODE code;
    const char *message;
} Error;

#endif
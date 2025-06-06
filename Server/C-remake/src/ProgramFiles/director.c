//
// Created by luis on 6/4/25.
//

#include "../HeaderFiles/director.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* input_handler(char* input) {
    char* inputToken = strtok(input, "|");
    if (strcmp(inputToken, "createuser")) {
        return "CreateUser";
    }else if (strcmp(inputToken, "login")) {
        return "Login";
    }else if(strcmp(inputToken, "deleteuser")) {
        return "DeleteUser";
    }else{
        return "Invalid Command";
    }
}
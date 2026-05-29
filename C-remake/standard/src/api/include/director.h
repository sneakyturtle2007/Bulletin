//
// Created by luis on 6/4/25.
//




#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "user_management.h"
#include "event_management.h"
#include "error_handling.h"
#include "string_utils.h"
#include "table.h"

Error input_handler(sqlite3 **db, char *input, String *output);

#endif //DIRECTOR_H

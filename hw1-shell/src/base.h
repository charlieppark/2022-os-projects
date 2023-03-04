#ifndef __BASE_H__
#define __BASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "color.h"
#include "error.h"

#define INPUT_BUF_SIZE 100

#define Validity char
#define VALID 1
#define INVALID 0
#define INIT -1

typedef struct _env {
    int login_name_max;
    int host_name_max;
    int path_max;
    int arg_max;
    char* login_name;
    char* host_name;
    char* current_path_abs;
    char* current_path;
    char* home_dir;
    char* history;
} Env;

void* malloc_s(size_t size, const char* const called_function);

#define DEBUG
#undef DEBUG

#endif //!__BASE_H__
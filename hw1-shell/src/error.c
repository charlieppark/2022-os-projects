#include "error.h"

void print_error(const char* const func_name, const char* const message) {
    if (errno > 0) {
        fprintf(stderr, "ERROR: [%s] %s : %s\n", func_name, message, strerror(errno));
    } else if (errno == 0) {
        fprintf(stderr, "ERROR: [%s] %s\n", func_name, message);
    } else {
        fprintf(stderr, "ERROR: [%s] %s : Caused by invalid errno value. KILL\n", func_name, message);
        exit(-1);
    }
}
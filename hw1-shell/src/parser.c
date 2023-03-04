#include "parser.h"

char** parser(const char* const input_string, int* size, const Env env) {
    char* delimiter = " \\\t";

    char* copied = malloc_s(sizeof(*copied) * env.arg_max, __func__);

    strcpy(copied, input_string);

    List* list = l_init();

    char* save_ptr;

    char* ptr = strtok_r(copied, delimiter, &save_ptr);
    
    while (ptr != NULL) {
        l_insert(list, ptr);
        ptr = strtok_r(NULL, delimiter, &save_ptr);
    }

    *size = list->size + 1;

    char** arr = malloc_s(sizeof(*arr) * (*size), __func__);

    for (int i = 0; i < (*size) - 1; i++) {
        arr[i] = l_get(list, 0);
        l_remove(list, 0);
    }

    arr[*size - 1] = NULL;

    free(list);

    return arr;
}
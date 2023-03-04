#include "base.h"

void* malloc_s(size_t size, const char* const called_function) {
    if (!size) {
        return NULL;
    }
    
    void* ptr = malloc(size);

    if (!ptr) {
        print_error(called_function, "Failed to allocate");
        exit(EXIT_FAILURE);
    }

    return ptr;
}
/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file safe_alloc.c
*/

#include "safe_alloc.h"

void* safe_malloc(size_t bytes) {
    void *mem = malloc(bytes);

    if(mem == NULL) {
        printf("Failed to allocate %lu bytes.", bytes);

        exit(1);
    }

    return mem;
}

void* safe_realloc(void* mem, size_t bytes) {
    void *mem = realloc(mem, bytes);

    if(mem == NULL) {
        printf("Failed to reallocate %lu bytes.", bytes);

        exit(1);
    }

    return mem;
}

void safe_free(void* mem) {
    free(mem); // Does nothing else for now...
}
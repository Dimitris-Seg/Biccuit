/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file safe_alloc.h
*/

#ifndef SAFE_ALLOC_H
#define SAFE_ALLOC_H

#include <stdio.h>
#include <stdlib.h>

void* safe_malloc(size_t bytes);

void* safe_realloc(void* mem, size_t bytes);

void safe_free(void* mem);

#endif


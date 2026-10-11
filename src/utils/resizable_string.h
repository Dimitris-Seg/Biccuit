/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file resizable_string.h
*/

#ifndef RESIZABLE_STRING_H
#define RESIZABLE_STRING_H

#define RESIZABLE_STRING_DEFAULT_SIZE 8
#define RESIZABLE_STRING_GROW_INCREMENT 16

#include <stdlib.h>
#include "safe_alloc.h"
#include "logging.h"
#include "bcc_definitions.h"

typedef struct ResizableString ResizableString_t;

struct ResizableString{
    char* chars;
    size_t len;
    size_t size;
};

ResizableString_t* new_resizable_string();

ResizableString_t* resizable_string_destroy(ResizableString_t* str);

void resizable_string_append(ResizableString_t* str, char c);

string resizable_string_get(ResizableString_t* str);

string resizable_string_clear(ResizableString_t* str);

#endif
/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file resizable_string.c
*/

#include "resizable_string.h"
#include <string.h>

ResizableString_t* new_resizable_string() {
    ResizableString_t* str = (ResizableString_t*)safe_malloc(sizeof(ResizableString_t));

    str->len = 0;
    str->size = RESIZABLE_STRING_DEFAULT_SIZE;

    str->chars = safe_malloc(str->size);
}

ResizableString_t* resizable_string_destroy(ResizableString_t* str) {
    if(str->chars != NULL)
        safe_free(str->chars);
    safe_free(str);
}

void resizable_string_append(ResizableString_t* str, char c) {
    if(str -> chars == NULL) {
        ERROR("Attempting to append to uninitialized resizable string, aborting...");
        exit(1);
    }

    if(str->len+2 >= str -> size) {
        str -> size += RESIZABLE_STRING_GROW_INCREMENT;
        str->chars = (char*)safe_realloc(str->chars, str->size);
        str->chars[str->len++] = c;
        str->chars[str->len] = '\0'; 
    }
}

string resizable_string_get(ResizableString_t* str) {
    return str->chars;
}

string resizable_string_clear(ResizableString_t* str) {
    memset(str->chars, '\0', str->len);
    str->len = 0;
}
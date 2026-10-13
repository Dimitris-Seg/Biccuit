/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file list.c
*/

#include <string.h>
#include "logging.h"
#include "list.h"
#include "safe_alloc.h"

List_t* new_list(size_t element_size) {
    List_t* list = (List_t*)safe_malloc(sizeof(List_t));

    list->element_size = element_size;
    list->len = 0;
    list->size = LIST_DEFAULT_SIZE;

    list->elements = safe_malloc(list->size * sizeof(void*));

    return list;
}

List_t* new_preallocated_list(size_t element_size, size_t num_elements, void* reference_entry) {
    List_t* list = (List_t*)safe_malloc(sizeof(List_t));

    list->element_size = element_size;
    list->len = 0;
    list->size = num_elements;

    list->elements = safe_malloc(list->size * sizeof(void*));

    for(size_t i = 0; i < num_elements; ++i)
        list_append(list, reference_entry);

    return list;
}

void list_destroy(List_t* list) {
    if(list->elements != NULL) {
        list_clear(list);
        safe_free(list->elements);
    }
    
    safe_free(list);
}

void list_append(List_t* list, void* element) {
    if(list -> elements == NULL) {
        ERROR("Attempting to append to uninitialized list, aborting...");
        exit(1);
    }

    if(list->len+1 >= list -> size) {
        list -> size += LIST_GROW_INCREMENT;
        list->elements = (void*)safe_realloc(list->elements, list->size * sizeof(void*));
    }

    ListEntry_t* newEntry = (ListEntry_t*)safe_malloc(list->element_size);
    memcpy(newEntry, element, list->element_size);

    list->elements[list->len++] = (void*)newEntry;
}

void* list_get(List_t* list, size_t index) {
    if(index >= list->len) {
        ERROR("Attempting to access index %lu is out of bounds for list of length %lu", index, list->len);
        return NULL;
    }

    return list->elements[index];
}

void list_clear(List_t* list) {
    if(list->elements != NULL) {
        for(size_t i = 0; i < list->len; ++i) {
            ((ListEntry_t*)(list->elements[i]))->destroy_element(list->elements[i]);
        }
    }

    list->len = 0;
}

size_t list_len(List_t* list) {
    return list->len;
}

void* list_find(List_t* list, ListSearchPredicate predicate, void* data) {
    for(size_t i = 0; i < list->len; ++i) {
        if(predicate(list->elements[i], data)) {
            return list->elements[i];
        }
    }

    return NULL;
}
/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file list.h
*/

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include "bcc_definitions.h"

#define LIST_DEFAULT_SIZE 32
#define LIST_GROW_INCREMENT 16

#define LIST_ENTRY_BASE_MEMBERS ListDestructor destroy_element;
#define LIST_ENTRY_BASE_SIZE sizeof(ListEntry_t) 

typedef struct List List_t;
typedef struct ListEntry ListEntry_t;
typedef void(*ListDestructor)(void* entry);
typedef bool(*ListSearchPredicate)(void* entry, void* data);

struct List{
    void** elements;
    size_t element_size;
    size_t len;
    size_t size;
};

struct ListEntry {
    LIST_ENTRY_BASE_MEMBERS
};

List_t* new_list(size_t element_size);

List_t* new_preallocated_list(size_t element_size, size_t num_elements, void* reference_entry);

void list_destroy(List_t* list);

void list_append(List_t* list, void* element);

void* list_get(List_t* list, size_t index);

void list_clear(List_t* list);

size_t list_len(List_t* list);

void* list_find(List_t* list, ListSearchPredicate predicate, void* data);
#endif

/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file hashtable.h
*/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>

#define HT_NUM_BUCKETS 32

#define HTENTRY_BASE_MEMBERS char* key; void* next;
#define HTENTRY_BASE_SIZE sizeof(HTEntry_t) 

typedef struct Hashtable Hashtable_t;
typedef struct HTBucket HTBucket_t;
typedef struct HTEntry HTEntry_t;

struct Hashtable {
    HTBucket_t* buckets;
    int total_buckets;
};

struct HTBucket {
    void* list_head;
    size_t element_size;
};

struct HTEntry {
    HTENTRY_BASE_MEMBERS
};

//// Some useful hashtable entry types ////

struct HTEntryInt {
    HTENTRY_BASE_MEMBERS
    int value;
};

//// End of entry types ////


Hashtable_t* hashtable_create(size_t elementSize);
void hashtable_destroy(Hashtable_t* ht);

/**
 * @param ht The hashtable
 * @param entry The entry to be inserted, is must use a type which inherits HTENTRY_BASE_MEMBERS
 *  at the start of its definition.
 */
void hashtable_insert(Hashtable_t* ht, string key, void* val);

void* hashtable_find(Hashtable_t* ht, string key);

#endif
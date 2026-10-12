/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file hashtable.c
*/

#include <string.h>
#include "hashtable.h"
#include "safe_alloc.h"

///////
static int hash_string(string str);
static void bucket_insert(HTBucket_t* bucket, string key, void* val);
static void bucket_destroy(HTBucket_t* bucket);
static void* bucket_find(HTBucket_t* bucket, string key);
static void bucket_remove(HTBucket_t* bucket, string key);
///////

Hashtable_t* hashtable_create(size_t elementSize) {
    Hashtable_t* ht = (Hashtable_t*)safe_malloc(sizeof(Hashtable_t));
    
    ht->buckets = (HTBucket_t*)safe_malloc(HT_NUM_BUCKETS * sizeof(HTBucket_t));
    ht->total_buckets = HT_NUM_BUCKETS;

    for(int i = 0; i < ht->total_buckets; ++i) {
        ht->buckets[i] = (HTBucket_t) {
            .list_head = NULL,
            .element_size = elementSize
        };
    }

    return ht;
}

void hashtable_destroy(Hashtable_t* ht) {
    for(int i = 0; i < ht->total_buckets; ++i) {
        bucket_destroy(&ht->buckets[i]);
    }

    safe_free(ht->buckets);
    safe_free(ht);
}

void hashtable_insert(Hashtable_t* ht, string key, void* val) {
    int bucket_no = hash_string(key) % ht->total_buckets;

    HTBucket_t* bucket = &(ht->buckets[bucket_no]);

    bucket_insert(bucket, key, val);
}

void hashtable_remove(Hashtable_t* ht, string key) {
    int bucket_no = hash_string(key) % ht->total_buckets;

    HTBucket_t* bucket = &(ht->buckets[bucket_no]);

    bucket_remove(bucket, key);
}

void* hashtable_find(Hashtable_t* ht, string key) {
    int bucket_no = hash_string(key) % ht->total_buckets;

    HTBucket_t* bucket = &(ht->buckets[bucket_no]);

    return bucket_find(bucket, key);
}

static int hash_string(string str) {
    int hash = 0;

    while(*str != '\0') {
        hash += *str;
        ++str;
    }

    return hash;
}


static void bucket_insert(HTBucket_t* bucket, string key, void* val) {
    HTEntry_t* processedEntry = NULL;

    if(bucket->list_head != NULL) {
        HTEntry_t* it = bucket->list_head;
        while(it) {
            if(!strcmp(it->key, key)) {
                processedEntry = it;
                break;
            }

            it = it->next;
        }
    }

    if(processedEntry == NULL) {
        processedEntry = (HTEntry_t*)safe_malloc(bucket->element_size);
        memcpy(processedEntry, val, bucket->element_size);

        processedEntry->key = key;
        processedEntry->next = bucket->list_head;

        bucket->list_head = processedEntry;
    } else {
        memcpy((char*)processedEntry + HTENTRY_BASE_SIZE, (char*)val + HTENTRY_BASE_SIZE, bucket->element_size - HTENTRY_BASE_SIZE);
    }
}

static void bucket_remove(HTBucket_t* bucket, string key) {
    HTEntry_t* processedEntry = NULL;

    if(bucket->list_head != NULL) {
        HTEntry_t* it = bucket->list_head;
        while(it) {
            if(!strcmp(it->key, key)) {
                processedEntry = it;
                break;
            }

            it = it->next;
        }
    }

    if(processedEntry != NULL) {
        bucket->list_head = processedEntry->next;
        safe_free(processedEntry);
    }
}

static void bucket_destroy(HTBucket_t* bucket) {
    HTEntry_t* it = bucket->list_head;
    HTEntry_t* prev = NULL;

    while(it) {
        prev = it;
        it = it->next;
        safe_free(prev);
    }
    
    bucket->list_head = NULL;
}

static void* bucket_find(HTBucket_t* bucket, string key) {
    HTEntry_t* it = bucket->list_head;

    while(it) {
        if(!strcmp(it->key, key)) {
            return it;
        }

        it = it->next;
    }

    return NULL;
}
/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file hashtable.c
*/

#include "hashtable.h"
#include <string.h>
#include "bcc_definitions.h"
#include "safe_alloc.h"

static int HashString(string str);
static void BucketInsert(HTBucket_t* bucket, string key, void* val);
static void BucketDestroy(HTBucket_t* bucket);
static void* BucketFind(HTBucket_t* bucket, string key);

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
        BucketDestroy(&ht->buckets[i]);
    }

    safe_free(ht->buckets);
    safe_free(ht);
}

void hashtable_insert(Hashtable_t* ht, string key, void* val) {
    int bucket_no = HashString(key) % ht->total_buckets;

    HTBucket_t* bucket = &(ht->buckets[bucket_no]);

    BucketInsert(bucket, key, val);
}

void* hashtable_find(Hashtable_t* ht, string key) {
    int bucket_no = HashString(key) % ht->total_buckets;

    HTBucket_t* bucket = &(ht->buckets[bucket_no]);

    return BucketFind(bucket, key);
}

static int HashString(string str) {
    int hash = 0;

    while(*str != '\0') {
        hash += *str;
        ++str;
    }

    return hash;
}


static void BucketInsert(HTBucket_t* bucket, string key, void* val) {
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

static void BucketDestroy(HTBucket_t* bucket) {
    HTEntry_t* it = bucket->list_head;
    HTEntry_t* prev = NULL;

    while(it) {
        prev = it;
        it = it->next;
        safe_free(prev);
    }
    
    bucket->list_head = NULL;
}

static void* BucketFind(HTBucket_t* bucket, string key) {
    HTEntry_t* it = bucket->list_head;

    while(it) {
        if(!strcmp(it->key, key)) {
            return it;
        }

        it = it->next;
    }

    return NULL;
}
/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file tokens.c
*/

#include <stdlib.h>
#include "tokens.h"
#include "../utils/hashtable.h"

static string base_tokens[]  = {
    [BT_CONNECTOR] = "->",
    [BT_DEFINE] = ":"
};

Hashtable_t* registered_tokens = NULL;

static void initialize_registered_tokens(void);

void register_token(string literal, int tokenid) {
    if(registered_tokens == NULL) {
        initialize_registered_tokens();
    }

    struct HTEntryInt newEntry = (struct HTEntryInt) {
        .value = tokenid
    };

    hashtable_insert(registered_tokens, literal, &newEntry);
}

int match_token(string literal) {
    if(registered_tokens == NULL)
        return BCC_INVALID_TOKEN;

    struct HTEntryInt* entry = (struct HTEntryInt*)hashtable_find(registered_tokens, literal);

    if(entry == NULL) 
        return BCC_INVALID_TOKEN;

    return entry->value;
}

static void initialize_registered_tokens() {
    registered_tokens = hashtable_create(sizeof(struct HTEntryInt));

    // Register base tokens
    int total_base_tokens = sizeof(base_tokens) / sizeof(string);

    for(int i = 0; i< total_base_tokens; ++i) {
        if(base_tokens[i] != NULL) {
            register_token(base_tokens[i], i);
        }
    }
}
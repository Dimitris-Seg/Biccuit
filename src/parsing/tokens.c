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

/////
static void _register_token(string literal, int tokenid, NodeDescriptor_t* node_descriptor);
/////

static string base_tokens[]  = {
    [BT_CONNECTOR] = "->",
    [BT_DEFINE] = ":",
    [BT_OPEN_INDEXER] = "[",
    [BT_CLOSE_INDEXER] = "]"
};

static string token_names[] = {
    "connector operator",
    "indexer open",
    "indexer close",
    "definition operator",
    "number",
    "id",
    "node"
};

Hashtable_t* registered_tokens = NULL;

static void initialize_registered_tokens(void);

void register_token(string literal, int tokenid) {
    _register_token(literal, tokenid, NULL);
}

void register_node(string literal, NodeDescriptor_t* node_descriptor) {
    _register_token(literal, BT_NODE, node_descriptor);
}

static void _register_token(string literal, int tokenid, NodeDescriptor_t* node_descriptor) {
    if(registered_tokens == NULL) {
        initialize_registered_tokens();
    }

    struct HTEntryTokenNode newEntry = (struct HTEntryTokenNode) {
        .tokenid = tokenid,
        .node_descriptor = node_descriptor
    };

    hashtable_insert(registered_tokens, literal, &newEntry);
}

void unregister_token(string literal) {
    if(registered_tokens != NULL) {
        hashtable_remove(registered_tokens, literal);
    }
}

string token_to_name(int tokenid) {
    if(tokenid < 0 || tokenid >= (int)(sizeof(token_names)/sizeof(string))) {
        return "invalid";
    }

    return token_names[tokenid];
}

struct HTEntryTokenNode* match_token(string literal) {
    if(registered_tokens == NULL)
        initialize_registered_tokens();

    struct HTEntryTokenNode* entry = (struct HTEntryTokenNode*)hashtable_find(registered_tokens, literal);

    if(entry == NULL) 
        return NULL;

    return entry;
}

static void initialize_registered_tokens() {
    registered_tokens = hashtable_create(sizeof(struct HTEntryTokenNode));

    // Register base tokens
    int total_base_tokens = sizeof(base_tokens) / sizeof(string);

    for(int i = 0; i< total_base_tokens; ++i) {
        if(base_tokens[i] != NULL) {
            register_token(base_tokens[i], i);
        }
    }
}
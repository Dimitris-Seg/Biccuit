/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file tokens.h
*/

#ifndef TOKENS_H
#define TOKENS_H

#include "../utils/hashtable.h"
#include "../utils/bcc_definitions.h"
#include "../nodes/node.h"

#define BCC_INVALID_TOKEN -1
#define BCC_NO_TOKEN -2
#define COMMENT_IDENTIFIER '#'
#define NL_IDENTIFIER '\n'
#define CR_IDENTIFIER '\r'

enum BaseToken {
    BT_CONNECTOR,
    BT_OPEN_INDEXER,
    BT_CLOSE_INDEXER,
    BT_DEFINE,
    BT_NUMBER,
    BT_ID,
    BT_NODE
};

struct HTEntryTokenNode {
    HTENTRY_BASE_MEMBERS
    int tokenid;
    NodeDescriptor_t* node_descriptor;
};

void register_token(string literal, int tokenid);
void register_node(string literal, NodeDescriptor_t* node_descriptor);
void unregister_token(string literal);
string token_to_name(int tokenid);
struct HTEntryTokenNode* match_token(string literal);

#endif
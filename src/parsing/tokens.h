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

#include "../utils/bcc_definitions.h"

#define BCC_INVALID_TOKEN -1

enum BaseToken {
    BT_CONNECTOR,
    BT_DEFINE
};

void register_token(string literal, int tokenid);

int match_token(string literal);

#endif
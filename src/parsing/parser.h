/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file parser.h
*/

#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "../utils/list.h"
#include "../utils/bcc_definitions.h"
#include "../nodes/node.h"

struct ListEntryToken {
    LIST_ENTRY_BASE_MEMBERS
    int tokenid;
    NodeDescriptor_t* node_descriptor;
    string value;
    int lineno;
};

List_t* parse_circuit(string filename);

#endif
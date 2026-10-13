/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 12/10/26
 * 
 * @file node.h
*/

#ifndef NODE_H
#define NODE_H

#include "../utils/bcc_definitions.h"
#include "../config.h"
#include "../utils/list.h"

typedef struct Node Node_t;
typedef struct NodeDescriptor NodeDescriptor_t;
typedef enum NodeArgType NodeArgType_t;
typedef struct NodeArg NodeArg_t;
typedef void(*NodeEvaluator)(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);

#define NODE_ANY_NUMBER_OF_INPUTS -1

struct Node {
    BCC_WIRE_TYPE output;
    void* data;
    List_t* args;
};

enum NodeArgType {
    BCC_ARG_INT,
    BCC_ARG_STRING
};

struct NodeArg {
    NodeArgType_t type;

    union {
        int int_val;
        string str_val;
    };
};

struct ListEntryNodeArg{
    LIST_ENTRY_BASE_MEMBERS
    NodeArg_t node_arg;
};

struct NodeDescriptor {
    int expected_inputs;
    NodeEvaluator evaluator;
};

#endif
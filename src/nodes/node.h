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

typedef struct Node Node_t;
typedef struct NodeDescriptor NodeDescriptor_t;
typedef void(*NodeEvaluator)(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);

#define NODE_ANY_NUMBER_OF_INPUTS -1

struct Node {
    BCC_WIRE_TYPE output;
    void* data;
};

struct NodeDescriptor {
    int expected_inputs;
    NodeEvaluator evaluator;
};

#endif
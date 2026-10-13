/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 12/10/26
 * 
 * @file graph.h
*/

#ifndef GRAPH_H
#define GRAPH_H

#include "../utils/list.h"
#include "../utils/hashtable.h"
#include "../nodes/node.h"

typedef struct Graph Graph_t;
typedef struct GraphNode GraphNode_t;

struct Graph {
    List_t* node_list;
    Hashtable_t* definitions;
    bool valid;
};

struct GraphNode {
    Node_t datanode;
    NodeDescriptor_t* node_descriptor;
    List_t* inputs;
    BCC_WIRE_TYPE true_output0;
    BCC_WIRE_TYPE true_output1;
};

struct ListEntryGraphNode {
    LIST_ENTRY_BASE_MEMBERS
    GraphNode_t* node;
};

struct HTEntryGraphNode {
    HTENTRY_BASE_MEMBERS
    GraphNode_t* node;
};

Graph_t create_graph(List_t* token_list);

void debug_log_graph(Graph_t* graph);

#endif

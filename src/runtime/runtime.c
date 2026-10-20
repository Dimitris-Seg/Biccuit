/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 12/10/26
 * 
 * @file runtime.c
*/

#include <stdio.h>
#include "input/polling.h"
#include "runtime.h"
/////
static void execute_graph_tick(Graph_t* graph, bool flip_bit, bool poll_input);
static BCC_WIRE_TYPE get_true_output(GraphNode_t* node, bool flip_bit);
static void set_flip_output(GraphNode_t* node, BCC_WIRE_TYPE value, bool flip_bit);
/////

bool graph_execution_aborted = false;

static void execute_graph_tick(Graph_t* graph, bool flip_bit, bool poll_input) {
    static int inputs[BCC_MAX_INPUTS];

    if(poll_input)
        _poll_keyboard();

    for(size_t i = 0; i < list_len(graph->node_list); ++i) {
        struct ListEntryGraphNode* entry_node = (struct ListEntryGraphNode*)list_get(graph->node_list, i);

        GraphNode_t* graph_node = entry_node->node;
        
        int input_count = 0;
        if(graph_node->node_descriptor->expected_inputs == NODE_ANY_NUMBER_OF_INPUTS) {
            input_count = list_len(graph_node->inputs);
        } else {
            input_count = graph_node->node_descriptor->expected_inputs;
        }

        for(int input_index = 0; input_index < input_count; ++input_index) {
            struct ListEntryGraphNode* input_node = (struct ListEntryGraphNode*)list_get(graph_node->inputs, input_index);

            if(input_node->node == NULL)
                inputs[input_index] = BCC_WIRE_DEFAULT;
            else
                inputs[input_index] = get_true_output(input_node->node, flip_bit);
        }

        graph_node->node_descriptor->evaluator(&graph_node->datanode, inputs, input_count);

        set_flip_output(graph_node, graph_node->datanode.output, flip_bit);
    }
}

void execute_n_graph_ticks(Graph_t* graph, int n, bool poll_input) {
    bool flip_bit = false;
    while(n > 0 && !graph_execution_aborted) {
        execute_graph_tick(graph, flip_bit, poll_input);
        flip_bit = !flip_bit;
        --n;
    }
}

void execute_graph_forever(Graph_t* graph, useconds_t tick_delay, bool poll_input) {
    bool flip_bit = false;
    while(!graph_execution_aborted) {
        execute_graph_tick(graph, flip_bit, poll_input);
        flip_bit = !flip_bit;

        usleep(tick_delay);
    }
}

void abort_graph_execution() {
    graph_execution_aborted = true;
}

static BCC_WIRE_TYPE get_true_output(GraphNode_t* node, bool flip_bit) {
    return flip_bit ? node->true_output1 : node->true_output0;
}

static void set_flip_output(GraphNode_t* node, BCC_WIRE_TYPE value, bool flip_bit) {
    if(flip_bit) 
        node->true_output0 = value;
    else
        node->true_output1 = value;
}
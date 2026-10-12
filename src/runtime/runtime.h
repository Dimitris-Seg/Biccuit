/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 12/10/26
 * 
 * @file runtime.h
*/

#ifndef RUNTIME_H
#define RUNTIME_H

#include "../graph/graph.h"
#include "../utils/list.h"

void execute_graph_tick(Graph_t* graph, bool flip_bit);
void execute_n_graph_ticks(Graph_t* graph, int n);
void execute_graph_forever(Graph_t* graph);

#endif
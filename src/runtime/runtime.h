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

#include <unistd.h>
#include "../graph/graph.h"
#include "../utils/list.h"

void execute_n_graph_ticks(Graph_t* graph, int n, bool poll_input);
void execute_graph_forever(Graph_t* graph, useconds_t tick_delay, bool poll_input);
void abort_graph_execution();

#endif
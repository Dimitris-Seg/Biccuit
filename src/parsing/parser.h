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
#include "../utils/bcc_definitions.h"
#include "../circuit/circuit.h"

bool parseCircuit(string filename, Circuit_t* circuit);

#endif
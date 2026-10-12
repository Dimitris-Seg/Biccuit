/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 12/10/26
 * 
 * @file plugin_loader.h
*/

#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <stdbool.h>
#include "../utils/bcc_definitions.h"

void load_plugins(string path, bool optional);

#endif
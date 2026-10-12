/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 12/10/26
 * 
 * @file plugin.h
*/

#ifndef PLUGIN_H
#define PLUGIN_H

#include "../utils/bcc_definitions.h"

#define PROVIDE_PLUGIN_ENDPOINT_NAME "provide_plugin"

typedef struct Plugin Plugin_t;
typedef Plugin_t(*ProvidePluginEndpoint)();
typedef void(*RegisterPluginCallback)();
typedef void(*UnregisterPluginCallback)();

struct Plugin {
    string name;
    RegisterPluginCallback registerPlugin;
    UnregisterPluginCallback unregisterPlugin;
};

#endif
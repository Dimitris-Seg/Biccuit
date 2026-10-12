/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 12/10/26
 * 
 * @file plugin_registry.h
*/

#ifndef PLUGIN_REGISTRY_H
#define PLUGIN_REGISTRY_H

#include "../utils/list.h"
#include "plugin.h"

struct ListEntryPlugin{
    LIST_ENTRY_BASE_MEMBERS
    Plugin_t plugin;
    bool registered;
};

void plugin_registry_add_plugin(Plugin_t plugin, string path);
void plugin_registry_register_all();
void plugin_registry_destroy();

#endif
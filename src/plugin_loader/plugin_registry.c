/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 12/10/26
 * 
 * @file plugin_registry.c
*/

#include <string.h>
#include "plugin_registry.h"
#include "../utils/logging.h"

static List_t* plugin_registry = NULL;

static void initialize_plugin_registry() {
    plugin_registry = new_list(sizeof(struct ListEntryPlugin));
}

static bool is_plugin_registered(void* entry, void* data) {
    struct ListEntryPlugin* plugin_entry = (struct ListEntryPlugin*)entry;
    string target_name = *((string*)data);

    return !strcmp(plugin_entry->plugin.name, target_name);
}

static void unregister_plugin_entry(void* entry) {
    struct ListEntryPlugin* plugin_entry = (struct ListEntryPlugin*)entry;

    if(plugin_entry->registered) {
        plugin_entry->plugin.unregisterPlugin();
    }
}

void plugin_registry_add_plugin(Plugin_t plugin, string path) {
    if(plugin_registry == NULL)
        initialize_plugin_registry();

    if(list_find(plugin_registry, is_plugin_registered, &(plugin.name)) != NULL) {
        WARN("A plugin with the name '%s' is already registered, skipping plugin %s", plugin.name, path);
        return;
    }

    struct ListEntryPlugin new_plugin = (struct ListEntryPlugin) {
        .destroy_element = unregister_plugin_entry,
        .plugin = plugin,
        .registered = false
    };

    list_append(plugin_registry, &new_plugin);

    DEBUG("Plugin '%s' loaded from %s.", plugin.name, path);
}

void plugin_registry_register_all() {
    if(plugin_registry == NULL)
        initialize_plugin_registry();

    int plugins_loaded = 0;

    for(size_t i = 0; i < list_len(plugin_registry); ++i) {
        struct ListEntryPlugin* plugin_entry = list_get(plugin_registry, i);

        if(!plugin_entry->registered) {
            plugin_entry->plugin.registerPlugin();
            ++plugins_loaded;
        }
    }

    if(plugins_loaded > 0) {
        DEBUG("Loaded %d plugin(s)!", plugins_loaded);
    }
}

void plugin_registry_destroy() {
    if(plugin_registry != NULL)
        list_destroy(plugin_registry);
}
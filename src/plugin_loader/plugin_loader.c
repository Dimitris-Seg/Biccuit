/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 12/10/26
 * 
 * @file plugin_loader.c
*/

#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include "plugin_loader.h"
#include "../utils/logging.h"
#include "plugin.h"
#include "plugin_registry.h"

#define PLUGIN_LOADER_MAX_PATH_SIZE 512

void load_plugins(string plugin_dir, bool optional) {
    DIR* dir = opendir(plugin_dir);

    if(!dir) {
        if(!optional)
            ERROR("Plugin directory '%s' not found, no plugins were loaded", plugin_dir);
        return;
    }

    struct dirent* entry;
    while((entry = readdir(dir)) != NULL) {
        // Look for .so files
        if(strstr(entry->d_name, ".so") != NULL) {
            char path[PLUGIN_LOADER_MAX_PATH_SIZE];

            // Construct path of entry from plugin dir
            snprintf(path, sizeof(path), "%s/%s", plugin_dir, entry->d_name);

            // Open the shared library
            void* handle = dlopen(path, RTLD_NOW);
            if(!handle) {
                ERROR("Failed to register plugin %s: %s", path, dlerror());
                continue;
            }

            // Hookup to the registration function
            ProvidePluginEndpoint provide_plugin = dlsym(handle, PROVIDE_PLUGIN_ENDPOINT_NAME);
            char* error = dlerror();
            if(error != NULL) {
                ERROR("Plugin %s is missing '%s': %s", path, PROVIDE_PLUGIN_ENDPOINT_NAME, error);
                dlclose(handle);
                continue;
            }

            plugin_registry_add_plugin(provide_plugin(), path);
        }
    }

    closedir(dir);
    
    plugin_registry_register_all();
}
/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 12/10/26
 * 
 * @file biccuit.c
*/

#include <stdio.h>
#include <string.h>
#include "utils/list.h"
#include "parsing/parser.h"
#include "graph/graph.h"
#include "runtime/runtime.h"
#include "utils/bcc_definitions.h"
#include "utils/logging.h"
#include "config.h"
#include "plugin_loader/plugin_loader.h"

#define IS_FLAG(str, flag)\
    (!strcmp(str, "-" flag))

///////
static void print_arg_error();
static void print_help(string argv[]);
static void flag_requires_arg(string flag, string need, int argc, int current_index);
/////

int main(int argc, string argv[]) {
    if(argc < 2) {
        print_arg_error();
        print_help(argv);
        return 1;
    }

    string input_filename = argv[1];
    string plugin_source = DEFAULT_PLUGIN_DIR;
    bool custom_plugin_dir = false;
    bool config_step = false;
    int number_of_ticks = -1;


    for(int i = 2; i < argc; ++i) {
        if(IS_FLAG(argv[i], "step")) {
            config_step = true;
        } else if(IS_FLAG(argv[i], "tick")) {
            flag_requires_arg("tick", "number of ticks", argc, i);
            number_of_ticks = atoi(argv[++i]);

            if(number_of_ticks == 0) {
                ERROR("Number of ticks must be an integer > 0");
                exit(1);
            }
        } else if(IS_FLAG(argv[i], "plugins")) {
            flag_requires_arg("plugins", "plugin source directory", argc, i);
            plugin_source = argv[++i];
            custom_plugin_dir = true;
        }
    }
    
    load_plugins(plugin_source, !custom_plugin_dir);

    List_t* parsed_tokens = parse_circuit(input_filename);

    // Uncomment for a debug dump of all partsed tokens
    // for(size_t i = 0; i < list_len(parsed_tokens); ++i) {
    //     struct ListEntryToken* token = list_get(parsed_tokens, i);
    //     INFO("Token #%lu: %d -> '%s'", i, token->tokenid, token->value);
    // }

    Graph_t graph = create_graph(parsed_tokens);

    if(!graph.valid) {
        ERROR("Graph is invalid, aborting execution...");
        return 1;
    }

    //debug_log_graph(&graph);

    NEWLINE();
    INFO(">> Starting execution <<\n");

    if(config_step) {
        // Handle stepping
    } else {
        if(number_of_ticks == -1) {
            execute_graph_forever(&graph);
        } else {
            execute_n_graph_ticks(&graph, number_of_ticks);
        }
    }
}

static void print_arg_error() {
    ERROR("Incorrect arguments provided, please refer to the proper usage below.");
}

static void print_help(string argv[]) {
    printf("Usage: %s <.bicc input file name>\n", argv[0]);
}

static void flag_requires_arg(string flag, string need, int argc, int current_index) {
    if(argc <= current_index + 1) {
        ERROR("Flag '%s' requires an additional to specify the %s", flag, need);
        exit(1);
    }
}
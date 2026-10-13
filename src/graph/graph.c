/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 12/10/26
 * 
 * @file graph.c
*/

#include <string.h>
#include "graph.h"
#include "../utils/logging.h"
#include "../parsing/parser.h"
#include "../parsing/tokens.h"
#include "../utils/safe_alloc.h"

/////
static void initialize_graph(Graph_t* graph);
static GraphNode_t* create_graph_node(Graph_t* graph, struct ListEntryToken* token, List_t* args);
static void destroy_graph_node(void* entry);
//static bool is_input_already_in_list(void* entry, void* data);
static struct HTEntryGraphNode* find_existing_id(Graph_t* graph, struct ListEntryToken* token);

static bool try_match_definition(Graph_t* graph, List_t* token_list, size_t* index, bool* failed);
static bool try_match_connection(Graph_t* graph, List_t* token_list, size_t* index, bool* failed);
static List_t* try_parse_arg_list(List_t* token_list, size_t* index, bool* failed);
/////

#define LOG_SEMANTIC_ERROR(lineno, error, ...)\
    ERROR("Line %d: " error, lineno, ##__VA_ARGS__)

Graph_t create_graph(List_t* token_list) {
    Graph_t graph = (Graph_t){
        .valid = true
    };

    if(token_list == NULL) {
        graph.valid = false;
        return graph;
    }

    initialize_graph(&graph);

    bool failed = false;

    for(size_t i = 0; i < list_len(token_list); ++i) {
        if(
            !try_match_definition(&graph, token_list, &i, &failed) &&
            !try_match_connection(&graph, token_list, &i, &failed)
        ) {
            struct ListEntryToken* failed_token = (struct ListEntryToken*)list_get(token_list, i);
            LOG_SEMANTIC_ERROR(failed_token->lineno, "Graph creation failed with semantic errors.");
            graph.valid = false;
            break;
        }
    }

    return graph;
}

static void initialize_graph(Graph_t* graph) {
    graph->node_list = new_list(sizeof(struct ListEntryGraphNode));
    graph->definitions = hashtable_create(sizeof(struct HTEntryGraphNode));
}

static bool try_match_definition(Graph_t* graph, List_t* token_list, size_t* index, bool* failed) {
    if(list_len(token_list) <= *index + 1)
        return false; // Not a definition without an assignment operator

    struct ListEntryToken* lop = (struct ListEntryToken*)list_get(token_list, *index);
    
    if(lop->tokenid != BT_ID)
        return false; // Not a definition without an id as the left operand

    struct ListEntryToken* op = (struct ListEntryToken*)list_get(token_list, *index + 1);    

    if(op->tokenid != BT_DEFINE)
        return false; // Not a definition without an assignment operator

    // At this point it must be an assignment so there's no going back
    
    if(list_len(token_list) <= *index + 2) {
        // No right operand, so it's a malformed assignment
        *failed = true;
        LOG_SEMANTIC_ERROR(op->lineno, "Definition requires a right operand but no right operand was found.");
        return false;
    }

    struct ListEntryToken* rop = (struct ListEntryToken*)list_get(token_list, *index + 2);  

    if(rop->tokenid != BT_NODE && rop ->tokenid != BT_ID) {
        // The right operand isn't of the right kind
        *failed = true;
        LOG_SEMANTIC_ERROR(rop->lineno, "Definition requires a node or id as the right operand but found %s.", token_to_name(rop->tokenid));
        return false;
    }

    //

    *index += 2;

    switch(rop->tokenid) {
        case BT_ID: {
            struct HTEntryGraphNode* existing_node = find_existing_id(graph, rop);

            if(existing_node == NULL) {
                *failed = true;
                return false;
            }

            hashtable_insert(graph->definitions, lop->value, existing_node);

            break;
        }
        case BT_NODE: {
            size_t args_index = *index + 1;
            List_t* args = try_parse_arg_list(token_list, &args_index, failed);

            if(args != NULL)
                *index = args_index;

            struct HTEntryGraphNode* new_node = &(struct HTEntryGraphNode) {
                .node = create_graph_node(graph, rop, args)
            };

            hashtable_insert(graph->definitions, lop->value, new_node);
            break;
        }
    }
    
    return true;
}

static bool try_match_connection(Graph_t* graph, List_t* token_list, size_t* index, bool* failed) {
    size_t temp_index = *index;

    if(list_len(token_list) <= temp_index + 1)
        return false; // Not a connection without a connection operator

    struct ListEntryToken* lop = (struct ListEntryToken*)list_get(token_list, temp_index);

    if(lop->tokenid != BT_NODE && lop->tokenid != BT_ID)
        return false; // Not a connection without an node or id as the left operand

    ++temp_index;

    GraphNode_t* lop_node = NULL;

    switch(lop->tokenid) {
        case BT_ID: {
            struct HTEntryGraphNode* existing_node = find_existing_id(graph, lop);

            if(existing_node == NULL) {
                *failed = true;
                return false;
            }

            lop_node = existing_node->node;

            break;
        }

        case BT_NODE: {
            List_t* args = try_parse_arg_list(token_list, &temp_index, failed);

            if(args != NULL)
                ++temp_index;
            
            lop_node = create_graph_node(graph, lop, args);
            break;
        }

    }

    struct ListEntryToken* op = (struct ListEntryToken*)list_get(token_list, temp_index);    

    if(op->tokenid != BT_CONNECTOR)
        return false; // Not a connection without a connector operator

    ++temp_index;

    // At this point it must be a connection so there's no going back

    if(list_len(token_list) <= temp_index) {
        // No right operand, so it's a malformed connection
        *failed = true;
        LOG_SEMANTIC_ERROR(op->lineno, "Connection requires a right operand but no right operand was found.");
        return false;
    }

    struct ListEntryToken* rop = (struct ListEntryToken*)list_get(token_list, temp_index);  

    if(rop->tokenid != BT_NODE && rop ->tokenid != BT_ID) {
        // The right operand isn't of the right kind
        *failed = true;
        LOG_SEMANTIC_ERROR(rop->lineno, "Connection requires a node or id as the right operand but found %s.", token_to_name(rop->tokenid));
        return false;
    }
    //

    GraphNode_t* rop_node = NULL;

    switch(rop->tokenid) {
        case BT_ID: {
            struct HTEntryGraphNode* existing_node = find_existing_id(graph, rop);

            if(existing_node == NULL) {
                *failed = true;
                return false;
            }

            rop_node = existing_node->node;

            break;
        }

        case BT_NODE: {
            size_t args_index = temp_index + 1;
            List_t* args = try_parse_arg_list(token_list, &args_index, failed);

            if(args != NULL)
                temp_index = args_index;

            rop_node = create_graph_node(graph, rop, args);
            break;
        }
    }

    int rop_index = -1;

    // Check if indexing exists
    if(list_len(token_list) > temp_index + 1) {
        struct ListEntryToken* indexer_open = (struct ListEntryToken*)list_get(token_list, temp_index + 1);

        if(indexer_open->tokenid == BT_OPEN_INDEXER) {
            // We have indexing for sure

            if(list_len(token_list) <= temp_index + 3) {
                *failed = true;
                LOG_SEMANTIC_ERROR(indexer_open->lineno, "Incomplete index formatting, expected opening indexer, number, closing indexer.");
                return false;
            }

            struct ListEntryToken* num = (struct ListEntryToken*)list_get(token_list, temp_index + 2);  

            if(num->tokenid != BT_NUMBER) {
                *failed = true;
                LOG_SEMANTIC_ERROR(indexer_open->lineno, "Expected an integer number for indexing, got %s.", token_to_name(num->tokenid));
                return false;
            }

            struct ListEntryToken* indexer_close = (struct ListEntryToken*)list_get(token_list, temp_index + 3);  

            if(indexer_close->tokenid != BT_CLOSE_INDEXER) {
                *failed = true;
                LOG_SEMANTIC_ERROR(indexer_open->lineno, "Expected closing indexer after number, got %s.", token_to_name(num->tokenid));
                return false;
            }

            rop_index = atoi(num->value);

            temp_index += 3;
        }
    }

    int rop_expected_inputs = rop_node->node_descriptor->expected_inputs;

    if(rop_expected_inputs > 0 && rop_index >= rop_expected_inputs) {
        *failed = true;
        LOG_SEMANTIC_ERROR(rop->lineno, "Node '%s' expects %d inputs but input at index %d was accessed.", rop->value, rop_expected_inputs, rop_index);
        return false;
    } else if(rop_expected_inputs == 0) {
        *failed = true;
        LOG_SEMANTIC_ERROR(rop->lineno, "Node '%s' expects no inputs and cannot be indexed", rop->value);
        return false;
    } else if(rop_index == -1) {
        rop_index = 0; // Default to input 0
    }

    if(rop_expected_inputs == NODE_ANY_NUMBER_OF_INPUTS) {
        // Use this to ensure there are no duplicate inputs
        //if(!list_find(rop_node->inputs, is_input_already_in_list, lop_node)) {

        struct ListEntryGraphNode* new_node = &(struct ListEntryGraphNode) {
            .node = lop_node
        };
        
        list_append(rop_node->inputs, new_node);
    } else if (rop_expected_inputs != 0) {
        struct ListEntryGraphNode* input_port = list_get(rop_node->inputs, rop_index);
        input_port->node = lop_node;
    }

    *index = temp_index;

    return true;
}

void debug_log_graph(Graph_t* graph) {
    List_t* node_list = graph->node_list;

    printf("\n** DEBUGGING GRAPH **\n\n");
    for(size_t i = 0; i < list_len(node_list); ++i) {
        struct ListEntryGraphNode* node = (struct ListEntryGraphNode*)list_get(node_list, i);
        int expected_inputs = node->node->node_descriptor->expected_inputs;

        DEBUG("Node at %p expects %d inputs:", node->node, expected_inputs);

        for(int ch = 0; ch < expected_inputs; ++ch) {
            struct ListEntryGraphNode* inp_node = (struct ListEntryGraphNode*)list_get(node->node->inputs, ch);
            int expected_inp_inputs = inp_node->node != NULL ? inp_node->node->node_descriptor->expected_inputs : 0;
            printf("\t*input at %p, expecting %d inputs.\n", inp_node->node, expected_inp_inputs);
        }
    }

    printf("\n** END OF GRAPH DEBUG **\n");
}

// Use this to to ensure there's no duplicate inputs
// static bool is_input_already_in_list(void* entry, void* data) {
//     struct ListEntryGraphNode* graph_entry = (struct ListEntryGraphNode*)entry;
//     GraphNode_t* target_node = (GraphNode_t*)data;

//     return graph_entry->node == target_node;
// }

static struct HTEntryGraphNode* find_existing_id(Graph_t* graph, struct ListEntryToken* token) {
    struct HTEntryGraphNode* existing_node = hashtable_find(graph->definitions, token->value);

    if(existing_node == NULL) {
        LOG_SEMANTIC_ERROR(
            token->lineno, 
            "Definition for '%s' not found, check whether you need to move that definition before this one or you forgot to define '%s'.",
            token->value, token->value
        );

        return NULL;
    }

    return existing_node;
}

static List_t* try_parse_arg_list(List_t* token_list, size_t* index, bool* failed) {
    if(list_len(token_list) <= *index) {
        return NULL;
    }

    struct ListEntryToken* args_open = (struct ListEntryToken*)list_get(token_list, *index);

    if(args_open->tokenid != BT_OPEN_ARGS) {
        return NULL; // Not an argument list with no open args token
    }

    // At this point it must be an argument list so there's no going back

    List_t* args = new_list(sizeof(struct ListEntryNodeArg));

    struct ListEntryToken* current_arg_entry = args_open;

    int arg_index = 1;
    bool argument_pending = true;
    int total_args = 0;
    while(true) {
        if(list_len(token_list) <= *index + arg_index) {
            *failed = true;
            LOG_SEMANTIC_ERROR(current_arg_entry->lineno, "Unterminated argument list.");
            return NULL;
        }

        current_arg_entry = (struct ListEntryToken*)list_get(token_list, *index + arg_index);

        if(current_arg_entry->tokenid == BT_CLOSE_ARGS && (!argument_pending || total_args == 0)) {
            *index += arg_index;
            return args;
        } else if(current_arg_entry->tokenid == BT_ARG_SEPARATOR && !argument_pending) {
            argument_pending = true;
            ++arg_index;
            continue;
        } else if(current_arg_entry->tokenid == BT_NUMBER) {
            struct ListEntryNodeArg arg = (struct ListEntryNodeArg) {
                .node_arg = (NodeArg_t) {
                    .int_val = atoi(current_arg_entry->value),
                    .type = BCC_ARG_INT
                }
            };
            list_append(args, &arg);
            argument_pending = false;
            ++total_args;
        } else if(current_arg_entry->tokenid == BT_STRING) {
            struct ListEntryNodeArg arg = (struct ListEntryNodeArg) {
                .node_arg = (NodeArg_t) {
                    .str_val = current_arg_entry->value,
                    .type = BCC_ARG_STRING
                }
            };
            list_append(args, &arg);
            argument_pending = false;
            ++total_args;
        } else {
            *failed = true;
            LOG_SEMANTIC_ERROR(current_arg_entry->lineno, "Expected argument of type number or string but got %s instead.", token_to_name(current_arg_entry->tokenid));
            return NULL;
        }
        ++arg_index;
    }

    return NULL;
}

static GraphNode_t* create_graph_node(Graph_t* graph, struct ListEntryToken* token, List_t* args) {
    GraphNode_t* new_node = (GraphNode_t*)safe_malloc(sizeof(GraphNode_t));

    int expected_inputs = token->node_descriptor->expected_inputs;
    if(expected_inputs > 0) {
        struct ListEntryGraphNode* reference_entry = &(struct ListEntryGraphNode) {
            .destroy_element = NULL,
            .node = NULL
        };

        new_node->inputs = new_preallocated_list(sizeof(struct ListEntryGraphNode), expected_inputs, reference_entry);
    } else if(expected_inputs == NODE_ANY_NUMBER_OF_INPUTS) {
        // Variadic input list (any number of inputs)
        new_node->inputs = new_list(sizeof(struct ListEntryGraphNode));
    } else {
        new_node->inputs = NULL;
    }

    new_node->datanode = (Node_t) {
        .data = NULL,
        .output = BCC_WIRE_DEFAULT,
        .args = args
    };

    new_node->true_output0 = BCC_WIRE_DEFAULT;
    new_node->true_output1 = BCC_WIRE_DEFAULT;
    new_node->node_descriptor = token->node_descriptor;

    struct ListEntryGraphNode* new_list_node = &(struct ListEntryGraphNode) {
        .destroy_element = destroy_graph_node,
        .node = new_node,
    };

    list_append(graph->node_list, new_list_node);

    return new_node;
}

static void destroy_graph_node(void* entry) {
    struct ListEntryGraphNode* node_entry = (struct ListEntryGraphNode*)entry;
    list_destroy(node_entry->node->inputs);
    safe_free(node_entry->node);
}
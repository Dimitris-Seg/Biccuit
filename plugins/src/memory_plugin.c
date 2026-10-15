/**
 * @brief This is a plugin for the Biccuit circuit emulator.
 * 
 * @author Dimitris Segkesser
 * 
 * @date 15/10/26
 * 
 * @file memory_plugin.c
*/

#include <stdbool.h>
#include "../../src/utils/logging.h"
#include "../../src/plugin_loader/plugin.h"
#include "../../src/parsing/tokens.h"
#include "../../src/utils/safe_alloc.h"

/////
static void evaluate_reg(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void register_plugin();
static void unregister_plugin();
/////

Plugin_t provide_plugin() {
    return (Plugin_t) {
        .name = "Memory Plugin",
        .registerPlugin = register_plugin,
        .unregisterPlugin = unregister_plugin
    };
}

// Descriptors //

static NodeDescriptor_t reg_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_reg
};

// Evaluators //

static void evaluate_reg(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    (void)inputs;
    (void)input_count;

    int value = 0;

    if(self->data == NULL) {
        int* reg_value = (int*)safe_malloc(sizeof(int));
        self->data = reg_value;

        if(self->args != NULL && list_len(self->args) > 0) {
            struct ListEntryNodeArg* arg = list_get(self->args, 0);
    
            if(arg->node_arg.type == BCC_ARG_INT) {
                value = arg->node_arg.int_val;
            }
        }

        *((int*)self->data) = value;
    }

    if(inputs[1]) {
        *((int*)self->data) = inputs[0];
    }

    self->output = *((int*)self->data);
}

//

static void register_plugin() {
    register_node("REG", &reg_descriptor);
}

static void unregister_plugin() {
    unregister_token("REG");
}

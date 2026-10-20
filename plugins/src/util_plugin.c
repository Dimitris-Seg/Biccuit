/**
 * @brief This is a plugin for the Biccuit circuit emulator.
 * 
 * @author Dimitris Segkesser
 * 
 * @date 20/10/26
 * 
 * @file util_plugin.c
*/

#include <stdbool.h>
#include "../../src/plugin_loader/plugin.h"
#include "../../src/parsing/tokens.h"

/////
static void evaluate_fanout(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void register_plugin();
static void unregister_plugin();
/////

Plugin_t provide_plugin() {
    return (Plugin_t) {
        .name = "Util Plugin",
        .registerPlugin = register_plugin,
        .unregisterPlugin = unregister_plugin
    };
}

// Descriptors //

static NodeDescriptor_t fanout_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 1,
    .evaluator = evaluate_fanout
};

// Evaluators //

static void evaluate_fanout(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    (void)input_count;
    
    self->output = inputs[0];
}

//

static void register_plugin() {
    register_node("FANOUT", &fanout_descriptor);
}

static void unregister_plugin() {
    unregister_token("FANOUT");
}

/**
 * @brief This is a plugin for the Biccuit circuit emulator.
 * 
 * @author Dimitris Segkesser
 * 
 * @date 12/10/26
 * 
 * @file bool_plugin.c
*/

#include <stdbool.h>
#include "../../src/plugin_loader/plugin.h"
#include "../../src/parsing/tokens.h"

/////
static void evaluate_and(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_or(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_true(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void register_plugin();
static void unregister_plugin();
/////

Plugin_t provide_plugin() {
    return (Plugin_t) {
        .name = "Bool Plugin",
        .registerPlugin = register_plugin,
        .unregisterPlugin = unregister_plugin
    };
}

// Descriptors //

static NodeDescriptor_t and_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_and
};

static NodeDescriptor_t or_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_or
};

static NodeDescriptor_t constant_true_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 0,
    .evaluator = evaluate_true
};

// Evaluators //

static void evaluate_and(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    bool result = true;

    for(int i = 0; i < input_count; ++i) {
        result = result && inputs[i];
    }

    self->output = result;
}

static void evaluate_or(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    bool result = false;

    for(int i = 0; i < input_count; ++i) {
        result = result || inputs[i];
    }

    self->output = result;
}


static void evaluate_true(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    (void)inputs;
    (void)input_count;
    self->output = true;
}

//

static void register_plugin() {
    register_node("AND", &and_descriptor);
    register_node("OR", &or_descriptor);
    register_node("TRUE", &constant_true_descriptor);
}

static void unregister_plugin() {
    unregister_token("AND");
    unregister_token("OR");
    unregister_token("TRUE");
}

/**
 * @brief This is a plugin for the Biccuit circuit emulator.
 * 
 * @author Dimitris Segkesser
 * 
 * @date 15/10/26
 * 
 * @file math_plugin.c
*/

#include <stdbool.h>
#include "../../src/utils/logging.h"
#include "../../src/plugin_loader/plugin.h"
#include "../../src/parsing/tokens.h"

/////
static void evaluate_add(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_subtract(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_opposite(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_multiply(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_divide(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_number(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void register_plugin();
static void unregister_plugin();
/////

Plugin_t provide_plugin() {
    return (Plugin_t) {
        .name = "Math Plugin",
        .registerPlugin = register_plugin,
        .unregisterPlugin = unregister_plugin
    };
}

// Descriptors //

static NodeDescriptor_t add_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_add
};

static NodeDescriptor_t subtract_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_subtract
};

static NodeDescriptor_t opposite_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 1,
    .evaluator = evaluate_opposite
};

static NodeDescriptor_t multiply_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_multiply
};

static NodeDescriptor_t divide_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_divide
};

static NodeDescriptor_t number_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 0,
    .evaluator = evaluate_number
};

// Evaluators //

static void evaluate_add(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    int result = 0;

    for(int i = 0; i < input_count; ++i) {
        result += inputs[i];
    }

    self->output = result;
}

static void evaluate_subtract(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    int result = inputs[0];

    for(int i = 1; i < input_count; ++i) {
        result -= inputs[i];
    }

    self->output = result;
}

static void evaluate_opposite(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    (void)input_count;
    self->output = -inputs[0];
}

static void evaluate_multiply(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    int result = inputs[0];

    for(int i = 1; i < input_count; ++i) {
        result *= inputs[i];
    }

    self->output = result;
}

static void evaluate_divide(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    int result = inputs[0];

    for(int i = 1; i < input_count; ++i) {
        if(inputs[i] == 0) {
            ERROR("Division by 0, result is undefined");
            result = 0;
        } else {
            result /= inputs[i];
        }
    }

    self->output = result;
}

static void evaluate_number(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    (void)inputs;
    (void)input_count;

    int value = 0;

    if(list_len(self->args) > 0) {
        struct ListEntryNodeArg* arg = list_get(self->args, 0);

        if(arg->node_arg.type == BCC_ARG_INT) {
            value = arg->node_arg.int_val;
        }
    }

    self->output = value;
}

//

static void register_plugin() {
    register_node("ADD", &add_descriptor);
    register_node("SUBTRACT", &subtract_descriptor);
    register_node("OPPOSITE", &opposite_descriptor);
    register_node("MULTIPLY", &multiply_descriptor);
    register_node("DIVIDE", &divide_descriptor);
    register_node("NUM", &number_descriptor);
}

static void unregister_plugin() {
    unregister_token("ADD");
    unregister_token("SUBTRACT");
    unregister_token("OPPOSITE");
    unregister_token("MULTIPLY");
    unregister_token("DIVIDE");
    unregister_token("NUM");
}

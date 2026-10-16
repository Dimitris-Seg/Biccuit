/**
 * @brief This is a plugin for the Biccuit circuit emulator.
 * 
 * @author Dimitris Segkesser
 * 
 * @date 12/10/26
 * 
 * @file comparisons_plugin.c
*/

#include <stdbool.h>
#include "../../src/plugin_loader/plugin.h"
#include "../../src/parsing/tokens.h"

/////
static void evaluate_geq(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_gt(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_leq(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_lt(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_eq(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_neq(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_mux(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void register_plugin();
static void unregister_plugin();
/////

Plugin_t provide_plugin() {
    return (Plugin_t) {
        .name = "Comparisons Plugin",
        .registerPlugin = register_plugin,
        .unregisterPlugin = unregister_plugin
    };
}

// Descriptors //

static NodeDescriptor_t geq_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_geq
};

static NodeDescriptor_t gt_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_gt
};

static NodeDescriptor_t leq_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_leq
};

static NodeDescriptor_t lt_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_lt
};

static NodeDescriptor_t eq_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_eq
};

static NodeDescriptor_t neq_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 2,
    .evaluator = evaluate_neq
};

static NodeDescriptor_t mux_descriptor = (NodeDescriptor_t) {
    .expected_inputs = -1,
    .evaluator = evaluate_mux
};

// Evaluators //

static void evaluate_geq(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    bool result = true;

    int comparison_value = inputs[0];
    for(int i = 1; i < input_count; ++i) {
        result = result && (comparison_value >= inputs[i]);
    }

    self->output = result;
}

static void evaluate_gt(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    bool result = true;

    int comparison_value = inputs[0];
    for(int i = 1; i < input_count; ++i) {
        result = result && (comparison_value > inputs[i]);
    }

    self->output = result;
}

static void evaluate_leq(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    bool result = true;

    int comparison_value = inputs[0];
    for(int i = 1; i < input_count; ++i) {
        result = result && (comparison_value <= inputs[i]);
    }

    self->output = result;
}

static void evaluate_lt(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    bool result = true;

    int comparison_value = inputs[0];
    for(int i = 1; i < input_count; ++i) {
        result = result && (comparison_value < inputs[i]);
    }

    self->output = result;
}

static void evaluate_eq(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    bool result = true;

    int comparison_value = inputs[0];
    for(int i = 1; i < input_count; ++i) {
        result = result && (comparison_value == inputs[i]);
    }

    self->output = result;
}

static void evaluate_neq(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    bool result = true;

    int comparison_value = inputs[0];
    for(int i = 1; i < input_count; ++i) {
        result = result && (comparison_value != inputs[i]);
    }

    self->output = result;
}

static void evaluate_mux(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    int selection = 0;

    if(self->args != NULL && list_len(self->args) > 0) {
        struct ListEntryNodeArg* arg = list_get(self->args, 0);

        if(arg->node_arg.type == BCC_ARG_INT) {
            selection = arg->node_arg.int_val;
        }
    }

    if(input_count != 0) {

        int selector = inputs[0];

        if(selector < input_count - 1) {
            selection = inputs[selector + 1];
        }
    }

    self->output = selection;
}

//

static void register_plugin() {
    register_node("GEQ", &geq_descriptor);
    register_node("GT", &gt_descriptor);
    register_node("LEQ", &leq_descriptor);
    register_node("LT", &lt_descriptor);
    register_node("EQ", &eq_descriptor);
    register_node("NEQ", &neq_descriptor);
    register_node("MUX", &mux_descriptor);
}

static void unregister_plugin() {
    unregister_token("GEQ");
    unregister_token("GT");
    unregister_token("LEQ");
    unregister_token("LT");
    unregister_token("EQ");
    unregister_token("NEQ");
    unregister_token("MUX");
}

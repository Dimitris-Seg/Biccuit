/**
 * @brief This is a plugin for the Biccuit circuit emulator.
 * 
 * @author Dimitris Segkesser
 * 
 * @date 12/10/26
 * 
 * @file display_plugin.c
*/

#include <stdio.h>
#include <stdbool.h>
#include "../../src/plugin_loader/plugin.h"
#include "../../src/parsing/tokens.h"

/////
static void evaluate_display(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void register_plugin();
static void unregister_plugin();
/////

Plugin_t provide_plugin() {
    return (Plugin_t) {
        .name = "Display Plugin",
        .registerPlugin = register_plugin,
        .unregisterPlugin = unregister_plugin
    };
}

// Descriptors //

static NodeDescriptor_t display_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 1,
    .evaluator = evaluate_display
};

// Evaluators //

static void evaluate_display(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    (void)input_count;
    printf("* displayed value: %d\n", inputs[0]);
    self->output = inputs[0];
}
//

static void register_plugin() {
    register_node("DISPLAY", &display_descriptor);
}

static void unregister_plugin() {
    unregister_token("DISPLAY");
}

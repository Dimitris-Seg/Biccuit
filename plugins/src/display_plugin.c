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
#include "../../src/utils/logging.h"
#include "../../src/utils/safe_alloc.h"
#include "../../src/plugin_loader/plugin.h"
#include "../../src/parsing/tokens.h"

/////
static void evaluate_display(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void register_plugin();
static void unregister_plugin();
/////

static int display_index = 0;

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

static int total_displays = 0;
static bool initialized_frames = false;

static void evaluate_display(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    (void)input_count;

    if(self->data == NULL) {
        int* display_id = (int*)safe_malloc(sizeof(int));
        *display_id = display_index++;
        self->data = display_id;
        total_displays = display_index;
    }

    int display_id = *((int*)self->data);

    if(display_id == 0) {
        if(initialized_frames) {
            printf("\033[%dA", total_displays); // Move cursor up by total display lines
        }
        initialized_frames = true;
    }

    // Print the line, clear to the end of the line, and move to the next line
    printf("\rDisplay #%d -> value: %d\033[K\n", display_id, inputs[0]);
    fflush(stdout);
    //printf("* displayed value: %d\n", inputs[0]);
    self->output = inputs[0];
}
//

static void register_plugin() {
    register_node("DISPLAY", &display_descriptor);
}

static void unregister_plugin() {
    unregister_token("DISPLAY");
}

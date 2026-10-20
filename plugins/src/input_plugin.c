/**
 * @brief This is a plugin for the Biccuit circuit emulator.
 * 
 * @author Dimitris Segkesser
 * 
 * @date 20/10/26
 * 
 * @file input_plugin.c
*/

#include <stdbool.h>
#include "../../src/utils/logging.h"
#include "../../src/utils/safe_alloc.h"
#include "../../src/plugin_loader/plugin.h"
#include "../../src/parsing/tokens.h"
#include "../../src/input/polling.h"

/////
static void _evaluate_keydown(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count, bool raw);
static void evaluate_keydown(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_keypress(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static void evaluate_keytoggle(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count);
static int get_keychar_from_str(string keychar_str);
static void register_plugin();
static void unregister_plugin();
/////

struct keytoggle_data {
    bool current_input;
    bool state;
};

Plugin_t provide_plugin() {
    return (Plugin_t) {
        .name = "Input Plugin",
        .registerPlugin = register_plugin,
        .unregisterPlugin = unregister_plugin
    };
}

// Descriptors //

static NodeDescriptor_t keydown_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 0,
    .evaluator = evaluate_keydown
};

static NodeDescriptor_t keypress_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 0,
    .evaluator = evaluate_keypress
};

static NodeDescriptor_t keytoggle_descriptor = (NodeDescriptor_t) {
    .expected_inputs = 0,
    .evaluator = evaluate_keytoggle
};

// Evaluators //

static void evaluate_keydown(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    _evaluate_keydown(self, inputs, input_count, false);
}

static void evaluate_keypress(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    _evaluate_keydown(self, inputs, input_count, true);
}

static void evaluate_keytoggle(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count) {
    (void)inputs;
    (void)input_count;

    _evaluate_keydown(self, inputs, input_count, true);

    bool keydown = self->output;

    if(self->data == NULL) {
        self->data = safe_malloc(sizeof(struct keytoggle_data));
        
        *((struct keytoggle_data*)self->data) = (struct keytoggle_data) {
            .current_input = keydown,
            .state = false
        };
    }

    struct keytoggle_data* data = self->data;

    if(!data -> current_input && keydown) {
        data->current_input = true;
        data->state = !data->state;
    }

    data->current_input = keydown;

    self->output = data->state;
}

//

static void _evaluate_keydown(Node_t* self, const BCC_WIRE_TYPE* inputs, int input_count, bool raw) {
    (void)inputs;
    (void)input_count;

    bool keydown = is_any_key_pressed(raw);

    if(self->args != NULL) {
        for(size_t arg_i = 0; arg_i < list_len(self->args); ++arg_i) {
            struct ListEntryNodeArg* arg = list_get(self->args, arg_i);

            if(arg->node_arg.type == BCC_ARG_STRING) {
                int keychar = get_keychar_from_str(arg->node_arg.str_val);

                if(keychar == -1)
                    keydown = false;

                keydown = is_key_pressed(keychar, raw);

                if(keydown)
                    break;
            }
        }
    }

    self->output = keydown;
}

static int get_keychar_from_str(string keychar_str) {
    if(keychar_str != NULL) {
        if(keychar_str[0] == '\0')
            return '\0';

        if(keychar_str[1] != '\0') {
            ERROR("KEYDOWN expected a single character string as input, got '%s'", keychar_str);
            return -1;
        }

        return keychar_str[0];
    }

    return -1;
}

static void register_plugin() {
    register_node("KEYDOWN", &keydown_descriptor);
    register_node("KEYPRESS", &keypress_descriptor);
    register_node("KEYTOGGLE", &keytoggle_descriptor);
}

static void unregister_plugin() {
    unregister_token("KEYDOWN");
    unregister_token("KEYPRESS");
    unregister_token("KEYTOGGLE");
}

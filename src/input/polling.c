/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 19/10/26
 * 
 * @file polling.c
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "polling.h"
#include "../config.h"
#include "../utils/logging.h"

#define ASCII_RANGE_LEN 128
#define SHUTDOWN_CHAR '\x03'

//
static long get_elapsed_ms(struct timespec* start, struct timespec* end);
//

static bool logged_tick_keys[ASCII_RANGE_LEN] = {0};
static bool logged_tick_keys_raw[ASCII_RANGE_LEN] = {0};
static bool any_key_down = true;
static bool any_key_down_raw = true;
static struct timespec last_press_time[ASCII_RANGE_LEN] = {0};
static TerminalShutdownCallback shutdown_callback = NULL;

bool is_key_pressed(int keychar, bool raw) {
    if(!isascii(keychar)) {
        ERROR("Code %d does not represent an ascii character and cannot be used for polling.", keychar);
        return false;
    }

    if(raw)
        return logged_tick_keys_raw[keychar];

    return logged_tick_keys[keychar];
}

bool is_any_key_pressed(bool raw) {
    if(raw)
        return any_key_down_raw;
        
    return any_key_down;
}

void _poll_keyboard() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    memset(logged_tick_keys_raw, 0, ASCII_RANGE_LEN);
    any_key_down_raw = false;

    unsigned char c = 0;
    while(read(STDIN_FILENO, &c, 1) > 0) {
        if(c == SHUTDOWN_CHAR && shutdown_callback != NULL) {
            shutdown_callback();
        }

        if(isascii(c)) {
            last_press_time[c] = now;
            logged_tick_keys_raw[c] = true;
            any_key_down_raw = true;
        }
    }

    // Reset logged keys
    memset(logged_tick_keys, 0, ASCII_RANGE_LEN);
    any_key_down = false;


    for(int i = 0; i < ASCII_RANGE_LEN; ++i) {
        // If this key has ever been pressed
        if(last_press_time[i].tv_sec != 0) {
            long elapsed = get_elapsed_ms(&last_press_time[i], &now);

            if(elapsed < BCC_KEY_HOLD_TIMEOUT_MS) {
                logged_tick_keys[i] = true;
                any_key_down = true;
            }
        }
    }
}

void _set_shutdown_callback(TerminalShutdownCallback callback) {
    shutdown_callback = callback;
}

// 

static long get_elapsed_ms(struct timespec* start, struct timespec* end) {
    long seconds = end->tv_sec - start->tv_sec;
    long nanos = end->tv_nsec - start->tv_nsec;

    return seconds * 1e3 + nanos / 1e6 ;
}
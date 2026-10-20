/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 19/10/26
 * 
 * @file terminal.c
*/

#include "terminal.h"
#include "../runtime/runtime.h"

/////
static void atexit_terminal_handler();
/////

static struct termios orig_termios;
static TerminalMode_t current_mode = TM_CANON;

void set_terminal_mode(TerminalMode_t mode) {
    switch(mode) {
        case TM_RAW: {
            if(current_mode != TM_RAW) {
                if(tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
                    perror("tcgetattr failed");
                    return;
                }

                atexit(atexit_terminal_handler);

                struct termios raw_termios = orig_termios;

                raw_termios.c_lflag &= TERM_RAW_MODE_FLAGS;

                if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios) == -1) {
                    perror("tcsetattr failed");
                    return;
                }

                // Add nonblock flag
                int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
                fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
            }

            break;
        }
        case TM_CANON: {
            if(current_mode != TM_CANON) {
                tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
                
                // Remove nonblock flag
                int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
                fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
            }
            break;
        }
    }

    current_mode = mode;
}

static void atexit_terminal_handler() {
    set_terminal_mode(TM_CANON);
    abort_graph_execution();
}
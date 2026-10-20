/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 19/10/26
 * 
 * @file terminal.h
*/

#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define TERM_RAW_MODE_FLAGS ~(ECHO | ICANON | ISIG)

typedef enum TerminalMode TerminalMode_t;

enum TerminalMode {
    TM_CANON,
    TM_RAW
};

void set_terminal_mode(TerminalMode_t mode);

#endif
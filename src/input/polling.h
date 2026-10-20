/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 19/10/26
 * 
 * @file polling.h
*/

#ifndef POLLING_H
#define POLLING_H

#include <stdio.h>
#include <stdbool.h>

typedef void(*TerminalShutdownCallback)(void);

bool is_key_pressed(int keychar, bool raw);
bool is_any_key_pressed(bool raw);

void _poll_keyboard();
void _set_shutdown_callback(TerminalShutdownCallback callback);

#endif
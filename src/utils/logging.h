/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file logging.h
*/


#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define COLOR_RESET "\e[0m"

#define DEBUG(msg, ...) printf(BLU"[DEBUG]"COLOR_RESET " " msg"\n", ##__VA_ARGS__)
#define INFO(msg, ...) printf(GRN"[INFO]"COLOR_RESET " " msg"\n", ##__VA_ARGS__)
#define WARN(msg, ...) printf(YEL"[WARNING]"COLOR_RESET " " msg"\n", ##__VA_ARGS__)
#define ERROR(msg, ...) printf(RED"[ERROR]"COLOR_RESET " " msg"\n", ##__VA_ARGS__)
#define NEWLINE() printf("\n");
#endif
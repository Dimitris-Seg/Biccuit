/**
 * @brief This is part of the implementation of Biccuit, 
 * a simple, modular, programmable binary circuit emulator.
 * 
 * @author Dimitris Segkesser
 * @date 11/10/26
 * 
 * @file parser.c
*/

#include <errno.h>
#include <ctype.h>
#include "parser.h"
#include "tokens.h"
#include "../utils/logging.h"
#include "../utils/bcc_definitions.h"
#include "../utils/resizable_string.h"

bool parseCircuit(string filename, Circuit_t* circuit) {
    FILE* file = fopen(filename, "r");

    if(file == NULL) {
        ERROR("Failed opening file %s: %s", filename, strerror(errno));
        return false;
    }

    ResizableString_t* buffer = new_resizable_string();

    int lineno = 1;
    int linepos = 0;
    int c = '\0';

    while(isNotEOF(c = getc(file))) {
        bool matchedToken = false;

        if(isspace(c)) {
            int matchedToken = flushBuffer(buffer);

            if(matchedToken == BCC_INVALID_TOKEN) {
                ERROR("Parsing failed at line %d, index %d: Unexpected token '%d'", lineno, linepos, resizable_string_get(buffer));
                return false;
            }
            
            // TODO: Add to token list
            matchedToken = true;
        } else if(isSymbol(c)) {
            int matchedToken = flushBuffer(buffer);

            if(matchedToken != BCC_INVALID_TOKEN) {
                // TODO: Add to token list
                matchedToken = true;
            }
        }

        if(!matchedToken) {
            resizable_string_append(buffer, c);
        }
    }

    fclose(file);

    return true;
}

static bool isSymbol(char c) {
    return !(isalnum(c) || c == "_");
}

static int flushBuffer(ResizableString_t* buffer) {
    int matchedToken = match_token(resizable_string_get(buffer));

    if(matchedToken != BCC_INVALID_TOKEN) {
        resizable_string_clear(buffer);
    }

    return matchedToken;
}

static bool isNotEOF(int c) {
    return c != '\0' && c != EOF;
}
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
#include <string.h>
#include "parser.h"
#include "tokens.h"
#include "../utils/logging.h"
#include "../utils/bcc_definitions.h"
#include "../utils/resizable_string.h"

///////
static void append_to_tokenlist(List_t* tokenlist, ResizableString_t* buffer, struct HTEntryTokenNode* token, int lineno);
static void destroy_tokenlist_entry(void* entry);
static bool is_symbol(char c);
static int resolve_escape_character(int c);
static struct HTEntryTokenNode* flush_buffer(ResizableString_t* buffer, int explicit_tokenid);
static bool is_id(string str);
static bool is_int(string str);
static bool is_not_eof(int c);
static bool is_newline(int c);
static struct HTEntryTokenNode* flush_and_clear(ResizableString_t* buffer, List_t* token_list, int lineno, int explicit_tokenid);
///////

static struct HTEntryTokenNode NO_TOKEN;
static struct HTEntryTokenNode ID_TOKEN = (struct HTEntryTokenNode){.tokenid = BT_ID, .node_descriptor = NULL};
static struct HTEntryTokenNode NUM_TOKEN = (struct HTEntryTokenNode){.tokenid = BT_NUMBER, .node_descriptor = NULL};
static struct HTEntryTokenNode STR_TOKEN = (struct HTEntryTokenNode){.tokenid = BT_STRING, .node_descriptor = NULL};

List_t* parse_circuit(string filename) {
    FILE* file = fopen(filename, "r");

    if(file == NULL) {
        ERROR("Failed opening file %s: %s", filename, strerror(errno));
        return NULL;
    }

    List_t* token_list = new_list(sizeof(struct ListEntryToken));
    ResizableString_t* buffer = new_resizable_string();

    int lineno = 1;
    int linepos = 0;
    bool in_comment = false;
    bool in_string = false;
    int old_c = '\0';
    int c = '\0';

    while(is_not_eof(c = getc(file))) {
        bool token_matched = false;
        
        // Enter comment
        if(c == COMMENT_IDENTIFIER)
            in_comment = true;

        // Discard everything in this line
        if(in_comment && c != NL_IDENTIFIER && c != CR_IDENTIFIER) {
            continue;
        } else {
            in_comment = false;
        }

        if(c == STRING_IDENTIFIER) {
            if(in_string && old_c != STRING_ESCAPE_IDENTIFIER) {
                in_string = false;
                flush_and_clear(buffer, token_list, lineno, BT_STRING);
                continue;
            } else if(!in_string){
                in_string = !in_string;
                flush_and_clear(buffer, token_list, lineno, BCC_NO_TOKEN);
                continue;
            }
        } 

        if(in_string) {
            ++linepos;

            if(c == STRING_ESCAPE_IDENTIFIER && old_c != STRING_ESCAPE_IDENTIFIER) {
                old_c = c;
                continue;
            }

            
            if(old_c == STRING_ESCAPE_IDENTIFIER) {
                old_c = '\0';
                c = resolve_escape_character(c);
            }
            
            resizable_string_append(buffer, c);
            
            continue;
        }

        if(isspace(c)) {
            struct HTEntryTokenNode* token = flush_and_clear(buffer, token_list, lineno, BCC_NO_TOKEN);

            if(token == NULL) {
                ERROR("Parsing failed at line %d, index %d: Unexpected token '%s'", lineno, linepos, resizable_string_get(buffer));
                return NULL;
            }

            token_matched = token != &NO_TOKEN; 

            if(is_newline(c)){
                ++lineno;
                linepos = 0;
            }
        }
        
        if(
            !token_matched && 
            (
                is_symbol(c) || 
                (
                    resizable_string_len(buffer) > 0 && 
                    is_symbol(resizable_string_get(buffer)[resizable_string_len(buffer) - 1])
                )
            )
        ) {
            struct HTEntryTokenNode* token = flush_and_clear(buffer, token_list, lineno, BCC_NO_TOKEN);
            token_matched = token != NULL && token != &NO_TOKEN;
        }

        if(!isspace(c)) {
            resizable_string_append(buffer, c);
            ++linepos;
        }

        old_c = c;
    }

    flush_and_clear(buffer, token_list, lineno, BCC_NO_TOKEN);

    fclose(file);

    return token_list;
}

static int resolve_escape_character(int c) {
    switch(c) {
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 'b':
            return '\b';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        case '\\':
            return '\\';
        case '\"':
            return '\"';
        default:
            return c;
    }
}

static struct HTEntryTokenNode* flush_and_clear(ResizableString_t* buffer, List_t* token_list, int lineno, int explicit_tokenid) {
    struct HTEntryTokenNode* token = flush_buffer(buffer, explicit_tokenid);

    if(token != NULL && token != &NO_TOKEN) {
        // Add to token list
        append_to_tokenlist(token_list, buffer, token, lineno);
        resizable_string_clear(buffer);
    }

    return token;
}

static void append_to_tokenlist(List_t* tokenlist, ResizableString_t* buffer, struct HTEntryTokenNode* token, int lineno) {
    list_append(tokenlist, &(
        (struct ListEntryToken) {
            .destroy_element = destroy_tokenlist_entry,
            .tokenid = token->tokenid,
            .node_descriptor = token->node_descriptor,
            .value = strdup(resizable_string_get(buffer)),
            .lineno = lineno
        }
    ));
}

static void destroy_tokenlist_entry(void* entry) {
    struct ListEntryToken* tokenEntry = (struct ListEntryToken*)entry;
    safe_free(tokenEntry->value);
}

static bool is_symbol(char c) {
    return !(isalnum(c) || c == '_');
}

static struct HTEntryTokenNode* flush_buffer(ResizableString_t* buffer, int explicit_tokenid) {
    char* str = resizable_string_get(buffer);

    if(explicit_tokenid != BCC_NO_TOKEN) {
        switch(explicit_tokenid) {
            case BT_ID:
                return &ID_TOKEN;
            case BT_STRING:
                return &STR_TOKEN;
            case BT_NUMBER:
                return &NUM_TOKEN;
        }
    }

    if(str[0] == '\0')
        return &NO_TOKEN;

    struct HTEntryTokenNode* matched_token = match_token(str);

    if(matched_token == NULL && is_id(str)) {
        return &ID_TOKEN;
    }

    if(matched_token == NULL && is_int(str)) {
        return &NUM_TOKEN;
    }
    
    return matched_token;
}

static bool is_id(string str) {
    if(str[0] == '\0') return false;

    // id cannot start with a number
    if(isdigit(str[0])) return false;

    while(*str != '\0') {
        if(is_symbol(*str))
            return false;
        ++str;
    }

    return true;
}

static bool is_int(string str) {
    if(!strcmp(str, "0"))
        return true;

    int num = atoi(str);

    return num != 0;
}

static bool is_not_eof(int c) {
    return c != '\0' && c != EOF;
}

static bool is_newline(int c) {
    return c == '\n' || c == '\r';
}
#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdlib.h>
#include <stdio.h>

typedef enum{
    TKN_EOF = 0,
    TKN_MAIN,
    TKN_VOID,
    TKN_INT,
    TKN_CHAR,
    TKN_RETURN,
    TKN_L_PRAN,         // (
    TKN_R_PRAN,         // )
    TKN_L_CURL,         // {
    TKN_R_CURL,         // }
    TKN_CMMNT,          // //
} Token_type;

typedef struct token_{
    Token_type type;
    void* value;
    struct token_* next;
} token;

token* tokenize(char* path);
token* token_stream(char* buffer);
token* next_token(char* buffer_ptr);

#endif // _LEXER_H_

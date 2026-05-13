#ifndef __KERN_LEXER_H_
#define __KERN_LEXER_H_

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TKN_INT,
    TKN_INT_VALUE,
    TKN_CHAR,
    TKN_VOID,
    TKN_FLOAT,
    TKN_DOUBLE,

    TKN_EQ,    // =
    TKN_PLUS,  // +
    TKN_MINUS, // -

    TKN_L_CBRACK, // {
    TKN_R_CBRACK, // {
    TKN_L_PRAN,   // (
    TKN_R_PRAN,   // (
    TKN_COL,      // ,
    TKN_SEMCOL,   // ;
    TKN_VIRGOOL,  // ,

    TKN_RETURN, // return

    TKN_IDENTIFIER,

    TKN_EOF, // end of file
} token_type;

typedef struct _Token {
    token_type type;
    void* value;
    struct _Token* next;
} Token;

typedef struct _Lexer {
    char* buffer;
    uint32_t line;
    uint32_t column;
    uint32_t pos;
    uint32_t len;
    uint32_t token_count;
    const char* filename;
    struct _Token* head;
} Lexer;

Lexer* src_to_tkn(const char* filename);
Lexer* tokenizer(Lexer* l);
Token* make_token(Lexer* l);
Token* make_op_token(Lexer* l);
Token* make_res_token(Lexer* l);

#endif

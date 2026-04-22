#ifndef __lexer_h__
#define __lexer_h__
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

typedef enum {
    TKN_INT,
    TKN_CHAR,
    TKN_MAIN,

    TKN_L_CBRACK, // {
    TKN_R_CBRACK, // {
    TKN_L_PRAN,   // (
    TKN_R_PRAN,   // (

    TKN_EOF,      // end of file
}token_type;


typedef struct _Token{
    token_type type;
    void* value;
    struct _Token* next;
}Token;


typedef struct _Lexer{
    char* buffer;
    unsigned line; 
    unsigned column; 
    unsigned pos; 
    unsigned len; 
    const char* filename; 
    struct _Token* head;
}Lexer;

Lexer* src_to_tkn(const char* filename);
Lexer* tokenizer(Lexer*);

#endif

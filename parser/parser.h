#ifndef __KERN_PARSER_H_
#define __KERN_PARSER_H_

#include "../Lexer/lexer.h"
#include "../vector.h"
#include <string.h>

#define NUDE_FUNCTION 999


typedef struct ASTNode_ {
    void* data;
    token_type type;
} ASTNode;

typedef struct func_dcl_ {
    char* fname;
    ASTNode* return_type;
    ASTNode** params;
    int param_cont;
    int statement_cont;
    vector* statements;
} func_dcl;

ASTNode* parse_program(Token* token);
ASTNode* parse_function(Token* token);
ASTNode* parse_var(Token* token);
ASTNode* parse_block(Token* token);

#endif // __KERN_PARSER_H_

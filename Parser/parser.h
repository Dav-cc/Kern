#ifndef __KERN_PARSER_H_
#define __KERN_PARSER_H_

#include "../Lexer/lexer.h"
#include "../vector.h"
#include <string.h>

typedef enum{
    NODE_PROGRAM = 0,
    NODE_FUNCTION,
    NODE_VARIABLE
}ast_type;

typedef struct ASTnode {
    void* data; // allocated for type parsed in making ast(struct type)
    ast_type type;
} ASTnode;

typedef struct Program {
    vector* functions;
    vector* global_vars;
    size_t function_count;
    size_t globvar_count;
} Program;

typedef struct Func {
    char* func_name;
    vector* params;
    int param_count;
    void* data; // allocate for saving function data block
} Func;

typedef struct Var{
    token_type type;
    char* var_name;
} Var;

typedef struct Parser{
    Token* sign;
    Token* current;
}Parser;



ASTnode* parse_program(Parser* p);
ASTnode* parse_function(Parser* p, Token* retype);
ASTnode* parse_var(Parser* p, Token* type);
Parser* init_parser(Token* tkn_stream);
#endif // __KERN_PARSER_H_

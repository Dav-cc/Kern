#ifndef __KERN_PARSER_H_
#define __KERN_PARSER_H_

#include "../Lexer/lexer.h"
#include "../vector.h"

typedef enum {
    NODE_FUNC,
    NODE_VAR_DECLR,
    NODE_RETURN,
    NODE_BLOCK,
} node_type;

typedef struct ASTnode_ {
    node_type type;
    void* data;
} ASTnode;

typedef struct var_dcl_ {
    char* var_name;
    int value;
} var_dcl;


typedef struct parsed_type_ {
    token_type ret_type;
    char* id;
} parsed_type;

typedef struct func_dcl_ {
    char* func_name;
    token_type ret_type;
    // vector* params;
    parsed_type** params;
    // vector* statement;
    var_dcl** statement;
    int state_count;
    int param_count;
} func_dcl;

typedef struct Parser_ {
    Token* tkn;
    int len;
} Parser;

parsed_type* parse_type(Parser* p);
ASTnode** parse_program(Parser* p);
ASTnode* parse_function(Parser* p);
void advance(Parser* p);
#endif // __KERN_PARSER_H_

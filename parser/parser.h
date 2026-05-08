#ifndef __KERN_PARSER_H_
#define __KERN_PARSER_H_

#include "../Lexer/lexer.h"
#include "../vector.h"
#include <stdlib.h>

typedef enum {
    NODE_FUNC,
    NODE_PROGRAM,
    NODE_BLOCK,
    NODE_VAR,
    NODE_EXPRESSION,
    NODE_EOF,
} parser_type;

typedef struct ASTNode_ {
    parser_type type;
    void* data;
    unsigned int row;
    unsigned int column;
} ASTNode;


typedef struct function_node_ {
    char* fname;
    int params_len;
    struct function_node_** params;
    ASTNode* returna_type;
    vector* statement;

} function_node;

#endif // __KERN_PARSER_H_

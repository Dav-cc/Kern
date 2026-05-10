#ifndef __KERN_PARSER_H_
#define __KERN_PARSER_H_

#include "../Lexer/lexer.h"

typedef enum {
    NUDE_INT_LITERAL,
    NUDE_FLOAT_LITERAL,
    NUDE_FUNC_DECL,
    NUDE_VAR_DECL,
    NUDE_IDENT,
    NUDE_ASSIGN,
    NUDE_BINARY_OP,
    NUDE_RETURN,
} node_type;

typedef struct ASTnode_ {
    node_type type;
    void* data;
    // TYPE for type checker
} ASTnode;

typedef struct Parser_{
    Token* cur;
    Token* prev;
    // bool error
}Parser;

ASTnode* parse_program(Parser* p);

#endif // __KERN_PARSER_H_

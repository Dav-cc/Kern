#ifndef __KERN_PARSER_H_
#define __KERN_PARSER_H_

#include "../Lexer/lexer.h"
#include "../vector.h"

typedef enum {
    AST_PROGRAM,
    AST_FUNC_DEF,
    AST_VAR_DEF,
    AST_EXPR,
    AST_STMT,
    AST_SUBTRACT,
    AST_TIME,
    AST_DIVIDE,
    AST_ADD,
    AST_RETURN,
    AST_IDENTIFIER,
    AST_INT_VALUE,
} ast_type;

typedef struct ASTnode {
    ast_type type;
    void* value;
} ASTnode;

typedef struct Parser {
    Lexer* lexer;
    Token* current_token;
} Parser;

typedef struct FuncDef {
    char* name;
    vector* params;
    ASTnode* body;
} FuncDef;

#endif // __KERN_PARSER_H_

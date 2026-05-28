#ifndef __KERN_PARSER_H_
#define __KERN_PARSER_H_

#include <string.h>
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
    Token* current;
} Parser;

typedef struct FuncDef {
    char* name;
    vector* params;
    ASTnode* body;
    token_type return_type;
} FuncDef;

typedef struct VarDef {
    char* name;
    token_type type;
} VarDef;

typedef struct ProgramNode{
    int func_def_count;
    vector* func_def;
    int glob_var_def_count;
    vector* glob_var_def;
}ProgramNode;

ASTnode* parse_type(Parser* p);
ASTnode* parse_program(Parser* p);
ASTnode* parse_function(Parser* p);
ASTnode* parse_var(Parser* p);

#endif // __KERN_PARSER_H_

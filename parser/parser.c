#include "parser.h"

int match(Parser* p, token_type type) {
    if (p->current->type == type) {
        p->current = p->current->next;
        return 1;
    }
    return 0;
}

int is_datatype(Token* token) {
    return (token->type == TKN_INT || token->type == TKN_FLOAT || token->type == TKN_DOUBLE ||
            token->type == TKN_CHAR || token->type == TKN_VOID)
               ? 1
               : 0;
}

ASTnode* new_node(ast_type type, size_t value_size) {
    ASTnode* node = (ASTnode*)malloc(sizeof(ASTnode));
    node->type = type;
    node->value = malloc(value_size);
    return node;
}

ASTnode* parse_type(Parser* p) {
    VarDef* var = malloc(sizeof(VarDef));
    if (is_datatype(p->current)) {
        var->type = p->current->type;
        p->current = p->current->next;
        if (match(p, TKN_IDENTIFIER)) {
            var->name = strdup((char*)p->current->value);
            ASTnode* node = new_node(AST_VAR_DEF, sizeof(VarDef));
            node->value = var;
            return node;
        }
        return NULL;
    }
    return NULL;
}


ASTnode* parse_var(Parser* p) {
    VarDef* var = malloc(sizeof(VarDef));
    if (is_datatype(p->current)) {
        var->type = p->current->type;
        p->current = p->current->next;
        if (match(p, TKN_IDENTIFIER)) {
            var->name = strdup((char*)p->current->value);
            ASTnode* node = new_node(AST_VAR_DEF, sizeof(VarDef));
            node->value = var;
            return node;
        }
        return NULL;
    }
    return NULL;
}



ASTnode* parse_function(Parser* p) {
    FuncDef* fdef = malloc(sizeof(FuncDef));
    if (is_datatype(p->current)) {
        // fucntion return type
        fdef->return_type = p->current->type;
        p->current = p->current->next;
        if (match(p, TKN_IDENTIFIER)) {
            fdef->name = strdup((char*)p->current->value);
            p->current = p->current->next;
            if (match(p, TKN_L_PRAN)) {
                while (!match(p, TKN_R_PRAN)) {
                    fdef->params = vector_init(sizeof(ASTnode*));
                    if (is_datatype(p->current)) {
                        ASTnode* param = parse_type(p);
                        vector_push(fdef->params, param);
                        if (match(p, TKN_COMMA)) {
                            p->current = p->current->next;
                        }
                    }
                    return NULL;
                }
                p->current = p->current->next;
                if (match(p, TKN_L_CBRACK)) {
                    ASTnode* body = parse_program(p);
                    fdef->body = body;
                    if (match(p, TKN_R_CBRACK)) {
                        ASTnode* node = new_node(AST_FUNC_DEF, sizeof(FuncDef));
                        node->value = fdef;
                        return node;
                    }
                    return NULL;
                }
                return NULL;
            }
            return NULL;
        }
        return NULL;
    }
    return NULL;
}

ASTnode* parse_program(Parser* p) {
    // init program node struct
    ProgramNode* program = (ProgramNode*)malloc(sizeof(ProgramNode));
    program->func_def_count = 0;
    program->func_def = vector_init(sizeof(FuncDef));
    program->glob_var_def_count = 0;
    program->glob_var_def = vector_init(sizeof(ASTnode*));

    if (match(p, TKN_EOF)) {
        return NULL;
    }
    while (p->current->type != TKN_EOF) {

        if (is_datatype(p->current)) {
            if (p->current->next->type == TKN_IDENTIFIER) {
                // function definition or variable definition

                if (p->current->next->next->type == TKN_L_PRAN) {
                    // function definition
                    ASTnode* func = parse_function(p);
                    program->func_def_count++;
                    program->func_def = vector_init(sizeof(FuncDef));
                    vector_push(program->func_def, func);
                }
                if (p->current->next->next->type == TKN_SEMCOL ||
                    p->current->next->next->type == TKN_COMMA) {
                    // variable definition
                    ASTnode* var = parse_var(p);
                    program->glob_var_def_count++;
                    program->glob_var_def = vector_init(sizeof(ASTnode*));
                    vector_push(program->glob_var_def, var);
                }
            }
        }
    }
    // return head of AST tree
    ASTnode* node = new_node(AST_PROGRAM, sizeof(ProgramNode));
    node->value = node;
    node->type = AST_PROGRAM;
    return node;
}

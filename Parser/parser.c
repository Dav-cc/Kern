#include "parser.h"

ASTnode* new_ast(size_t sz, ast_type type) {
    ASTnode* n = (ASTnode*)malloc(sizeof(ASTnode));
    n->data = malloc(sz);
    n->type = type;
    return n;
}

int is_datatype(Token* tkn) {
    if (tkn->type == TKN_CHAR || tkn->type == TKN_INT || tkn->type == TKN_FLOAT ||
        tkn->type == TKN_DOUBLE || tkn->type == TKN_VOID) {
        return 1;
    }
    return 0;
}

Parser* init_parser(Token* tkn_stream) {
    Parser* p = (Parser*)malloc(sizeof(Parser));
    p->current = tkn_stream;
    p->sign = tkn_stream;
    return p;
}

ASTnode* parse_program(Parser* p) {
    Program* prog = {0};
    prog->function_count = 0;
    prog->functions = vector_init(sizeof(Func));
    prog->global_vars = vector_init(sizeof(Var));
    prog->globvar_count = 0;

    if (is_datatype(p->current)) { // it could be variable define or function return type
        p->sign = p->current;
        p->current = p->current->next;
        if (p->current->type ==
            TKN_IDENTIFIER) { // fuction identifier or variable name (have to save this name)
            p->current = p->current->next;
            if (p->current->type == TKN_L_PRAN) {
                ASTnode* func_node = parse_function(p, p->sign);
                vector_push(prog->functions, func_node);
                prog->function_count++;
            }
            if (p->current->type == TKN_EQ) {
                ASTnode* var_node = parse_var(p, p->sign);
                vector_push(prog->global_vars, var_node);
                prog->globvar_count++;
            }
        }
    }
    if (p->current == NULL || p->current->type == TKN_EOF) {
        ASTnode* res = new_ast(sizeof(Program), NODE_PROGRAM);
        res->data = prog;
        return res;
    }
    return NULL;
}

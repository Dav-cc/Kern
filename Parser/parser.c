#include "parser.h"

static void parser_error(Parser* p, const char* msg) {
    if (p->current && p->current->value) {
        fprintf(stderr, "parse error: %s (near '%s')\n", msg, (char*)p->current->value);
    } else {
        fprintf(stderr, "parse error: %s\n", msg);
    }
}

static Token* advance(Parser* p) {
    if (p->current) {
        p->current = p->current->next;
    }
    return p->current;
}

static int match(Parser* p, token_type type) {
    if (p->current && p->current->type == type) {
        advance(p);
        return 1;
    }
    return 0;
}

static Token* expect(Parser* p, token_type type, const char* what) {
    if (!p->current || p->current->type != type) {
        parser_error(p, what);
        return NULL;
    }
    Token* t = p->current;
    advance(p);
    return t;
}

static int parse_expression(Parser* p);
static int parse_primary(Parser* p);
static int parse_statement(Parser* p);
static int parse_block(Parser* p);

ASTnode* new_ast(ast_type type) {
    ASTnode* n = (ASTnode*)calloc(1, sizeof(ASTnode));
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
    Parser* p = (Parser*)calloc(1, sizeof(Parser));
    p->current = tkn_stream;
    p->sign = tkn_stream;
    return p;
}

// primary   : TKN_INT_VALUE | TKN_IDENTIFIER | '(' expression ')'
static int parse_primary(Parser* p) {
    if (!p->current) {
        return 0;
    }
    switch (p->current->type) {
        case TKN_INT_VALUE:
        case TKN_IDENTIFIER:
            advance(p);
            return 1;
        case TKN_L_PRAN:
            advance(p);
            if (!parse_expression(p)) {
                return 0;
            }
            return match(p, TKN_R_PRAN);
        default:
            return 0;
    }
}

// expression : primary (('+' | '-') primary)*
static int parse_expression(Parser* p) {
    if (!parse_primary(p)) {
        return 0;
    }
    while (p->current && (p->current->type == TKN_PLUS || p->current->type == TKN_MINUS)) {
        advance(p);
        if (!parse_primary(p)) {
            return 0;
        }
    }
    return 1;
}

static int parse_guard(Parser* p) {
    if (!match(p, TKN_L_PRAN)) {
        parser_error(p, "expected '('");
        return 0;
    }
    if (p->current && p->current->type != TKN_R_PRAN) {
        if (!parse_expression(p)) {
            return 0;
        }
    }
    while (p->current && p->current->type == TKN_SEMCOL) { // e.g. for(;;)
        advance(p);
        if (p->current && p->current->type != TKN_R_PRAN) {
            if (!parse_expression(p)) {
                return 0;
            }
        }
    }
    if (!match(p, TKN_R_PRAN)) {
        parser_error(p, "expected ')' to close the condition");
        return 0;
    }
    return 1;
}

// declaration : type identifier [',' identifier]* ['=' expression] ';'
static int parse_declaration(Parser* p) {
    if (!is_datatype(p->current)) {
        parser_error(p, "expected a data type");
        return 0;
    }
    advance(p);
    if (!expect(p, TKN_IDENTIFIER, "expected an identifier after the type")) {
        return 0;
    }
    while (p->current && p->current->type == TKN_COMMA) {
        advance(p);
        if (!expect(p, TKN_IDENTIFIER, "expected an identifier after ','")) {
            return 0;
        }
    }
    if (match(p, TKN_EQ)) {
        if (!parse_expression(p)) {
            parser_error(p, "invalid initializer");
            return 0;
        }
    }
    if (!match(p, TKN_SEMCOL)) {
        parser_error(p, "expected ';' at the end of the declaration");
        return 0;
    }
    return 1;
}

static int parse_statement(Parser* p) {
    if (!p->current) {
        return 0;
    }
    switch (p->current->type) {
        case TKN_L_CBRACK:
            return parse_block(p);
        case TKN_SEMCOL:
            advance(p);
            return 1;
        case TKN_RETURN:
            advance(p);
            if (p->current && p->current->type != TKN_SEMCOL && !parse_expression(p)) {
                return 0;
            }
            return match(p, TKN_SEMCOL);
        case TKN_BREAK:
        case TKN_CONTINUE:
            advance(p);
            return match(p, TKN_SEMCOL);
        case TKN_IF:
            advance(p);
            if (!parse_guard(p) || !parse_statement(p)) {
                return 0;
            }
            if (match(p, TKN_ELSE)) {
                return parse_statement(p);
            }
            return 1;
        case TKN_WHILE:
        case TKN_SWITCH:
            advance(p);
            if (!parse_guard(p)) {
                return 0;
            }
            return parse_statement(p);
        case TKN_FOR:
            advance(p);
            if (!parse_guard(p)) {
                return 0;
            }
            return parse_statement(p);
        case TKN_DO:
            advance(p);
            if (!parse_statement(p) || !match(p, TKN_WHILE) || !parse_guard(p)) {
                return 0;
            }
            return match(p, TKN_SEMCOL);
        default:
            if (is_datatype(p->current)) {
                return parse_declaration(p);
            }
            if (!parse_expression(p)) {
                parser_error(p, "expected a statement");
                return 0;
            }
            return match(p, TKN_SEMCOL);
    }
}

static int parse_block(Parser* p) {
    if (!match(p, TKN_L_CBRACK)) {
        parser_error(p, "expected '{'");
        return 0;
    }
    while (p->current && p->current->type != TKN_R_CBRACK) {
        if (!parse_statement(p)) {
            return 0;
        }
    }
    return match(p, TKN_R_CBRACK);
}

ASTnode* parse_function(Parser* p, Token* retype, Token* name) {
    Func* func = (Func*)calloc(1, sizeof(Func));
    func->return_type = retype->type;
    func->func_name = strdup((char*)name->value);
    func->params = vector_init(sizeof(ASTnode*));

    if (!match(p, TKN_L_PRAN)) {
        parser_error(p, "expected '(' after the function name");
        goto fail;
    }
    if (p->current && p->current->type != TKN_R_PRAN) {
        while (p->current && p->current->type != TKN_R_PRAN) {
            if (!is_datatype(p->current)) {
                parser_error(p, "expected a data type in the parameter list");
                goto fail;
            }
            Token* ptype = p->current;
            advance(p);
            if (!expect(p, TKN_IDENTIFIER, "expected a parameter name")) {
                goto fail;
            }
            Var* pty = (Var*)calloc(1, sizeof(Var));
            pty->type = ptype->type;
            pty->var_name = strdup((char*)ptype->next->value);
            ASTnode* pnode = new_ast(NODE_VARIABLE);
            pnode->data = pty;
            vector_push(func->params, pnode);
            func->param_count++;
            if (!match(p, TKN_COMMA) && p->current && p->current->type != TKN_R_PRAN) {
                parser_error(p, "expected ',' or ')' in the parameter list");
                goto fail;
            }
        }
    }
    if (!match(p, TKN_R_PRAN)) {
        parser_error(p, "expected ')' to close the parameter list");
        goto fail;
    }
    if (!parse_block(p)) {
        goto fail;
    }

    ASTnode* node = new_ast(NODE_FUNCTION);
    node->data = func;
    return node;

fail:
    free(func->func_name);
    free(func->params);
    free(func);
    return NULL;
}

ASTnode* parse_var(Parser* p, Token* type, Token* name) {
    Var* var = (Var*)calloc(1, sizeof(Var));
    var->type = type->type;
    var->var_name = strdup((char*)name->value);

    if (match(p, TKN_EQ)) {
        if (!parse_expression(p)) {
            parser_error(p, "invalid initializer");
            goto fail;
        }
    }
    if (!match(p, TKN_SEMCOL)) {
        parser_error(p, "expected ';' at the end of the variable declaration");
        goto fail;
    }

    ASTnode* node = new_ast(NODE_VARIABLE);
    node->data = var;
    return node;

fail:
    free(var->var_name);
    free(var);
    return NULL;
}

ASTnode* parse_program(Parser* p) {
    Program* prog = (Program*)calloc(1, sizeof(Program));
    prog->functions = vector_init(sizeof(ASTnode*));
    prog->global_vars = vector_init(sizeof(ASTnode*));

    while (p->current && p->current->type != TKN_EOF) {
        if (!is_datatype(p->current)) {
            parser_error(p, "expected a top-level declaration");
            return NULL;
        }
        Token* type = p->current;
        p->sign = type;
        advance(p);
        Token* name = expect(p, TKN_IDENTIFIER, "expected a name after the type");
        if (!name) {
            return NULL;
        }
        if (p->current && p->current->type == TKN_L_PRAN) {
            ASTnode* func_node = parse_function(p, type, name);
            if (!func_node) {
                return NULL;
            }
            vector_push(prog->functions, func_node);
            prog->function_count++;
        } else {
            ASTnode* var_node = parse_var(p, type, name);
            if (!var_node) {
                return NULL;
            }
            vector_push(prog->global_vars, var_node);
            prog->globvar_count++;
        }
    }

    ASTnode* res = new_ast(NODE_PROGRAM);
    res->data = prog;
    return res;
}

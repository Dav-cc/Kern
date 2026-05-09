#include "parser.h"

int HAS_MAIN = 0;

static int peek(Token* tkn) { return ((tkn->next)->type); }

ASTNode* new_node(int data_size) {
    ASTNode* n = (ASTNode*)malloc(sizeof(ASTNode));
    n->data = malloc(data_size);
    return n;
}

ASTNode* parse_type(Token* tkn) {
    ASTNode* node = new_node(sizeof(tkn->value));
    node->type = tkn->type;
    node->data = tkn->value;
    return node;
}
ASTNode* parse_program(Token* token) {
    Token* tkn = token;
    if (tkn->type == TKN_INT) { // data types (what functions return) soon we support pointers
        if ((peek(tkn)) == TKN_IDENTIFIER) {
            tkn = tkn->next;
            if (peek(tkn) == TKN_L_PRAN) {
                ASTNode* node = parse_function(token);
                return node;
            }
            if (peek(tkn) == TKN_EQ) {
                ASTNode* nod = parse_var(token);
                return nod;
            }
            return NULL;
        }
        return NULL;
    }
    return NULL;
}

ASTNode* parse_function(Token* token) {
    Token* tkn = token;
    func_dcl* func = (func_dcl*)malloc(sizeof(func_dcl));
    ASTNode* return_type = parse_type(tkn);

    func->return_type = return_type;
    tkn = tkn->next;
    func->fname = tkn->value;
    if ((strcmp(func->fname, "main") == 0)) {
        HAS_MAIN = 1;
    }
    tkn = tkn->next;

    if (tkn->type == TKN_L_PRAN) {
        tkn = tkn->next;
        if (tkn->type == TKN_R_PRAN) {
            func->param_cont = 0;
            func->params = NULL;
        }
        // NOTE: parse func args soon . . . 
        //
        // while(tkn != TKN_L_CBRACK){
        //     vector* vec = (vector*)malloc(sizeof(vector*)* func->param_cont);
        //     parse_var(tkn);
        // }
        tkn = tkn->next;
        if(tkn->type == TKN_L_CBRACK){
            ASTNode* block_node = parse_block(tkn);
        }
    } else {
        return NULL;
    }
}

ASTNode* parse_var(Token* token) {
    printf("im in var func\n");
}

ASTNode* parse_block(Token* token) {
    printf("im in block\n");
}

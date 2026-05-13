#include "parser.h"

token_type peek(Parser* p) { return ((p->tkn->next)->type); }

void advance(Parser* p) { p->tkn = p->tkn->next; }

int is_datatype(Parser* p) {
    return ((p->tkn->type == TKN_INT) || (p->tkn->type == TKN_CHAR) || (p->tkn->type == TKN_VOID) ||
            (p->tkn->type == TKN_DOUBLE) || (p->tkn->type == TKN_FLOAT))
               ? 1
               : 0;
}

// parsed_type* parse_type(Parser* p) {
//     // in here our p->tkn->type is on a data type
//     if (is_datatype(p)) {
//         parsed_type* returned_type = (parsed_type*)malloc(sizeof(parsed_type));
//         returned_type->ret_type = p->tkn->type;
//         p->tkn = p->tkn->next;
//         if (p->tkn->type == TKN_IDENTIFIER) {
//             returned_type->id = p->tkn->value;
//             p->tkn = p->tkn->next;
//         }
//         return returned_type;
//     }
//     return NULL;
// }

ASTnode* new_node(node_type tp, int data_size) {
    ASTnode* node = malloc(sizeof(ASTnode));
    node->type = tp;
    node->data = malloc(data_size);
    return node;
}

ASTnode** parse_program(Parser* p) {
    int funcs_count = 0;
    ASTnode** funcs = (ASTnode**)malloc(sizeof(ASTnode*) * 10);
    for (int i = 0; i < 10; i++) {
        funcs[i] = NULL;
    }

    /*
     *
     *  for simplicity in this version we only parse functions and blocks :
     *
     *  [NODE_FUNC](1) [STATEMENTS_VECTOR](1)
     *  [NODE_FUNC](2) [STATEMENTS_VECTOR](2)
     *  [NODE_FUNC](3) [STATEMENTS_VECTOR](3)
     *
     */

    while (((p->tkn)->type) != TKN_EOF) {
        funcs[funcs_count++] = parse_function(p);
    }
    return funcs;
}

ASTnode* parse_function(Parser* p) {
    func_dcl* fun = malloc(sizeof(func_dcl));
    if (is_datatype(p)) {
        fun->ret_type = p->tkn->type;
        advance(p);
        fun->func_name = p->tkn->value;
        advance(p);
        if (p->tkn->type == TKN_L_PRAN) {
            advance(p);
            parsed_type** param =
                (parsed_type**)malloc(sizeof(parsed_type*) * 5); // max 5 args for every func
            for (int i = 0; i < 5; i++) {
                param[i] = NULL;
            }
            int y = 0;
            while (p->tkn->type != TKN_R_PRAN) {

                parsed_type* new_param = (parsed_type*)malloc(sizeof(parsed_type));
                new_param->ret_type = p->tkn->type;
                advance(p);
                new_param->id = p->tkn->value;
                advance(p);

                param[y++] = new_param;
                if (p->tkn->type == TKN_VIRGOOL)
                    advance(p);
            }
            fun->params = param;
            fun->param_count = y;
            advance(p);
        }
        advance(p);     // advance TKN_R_PRAN
        if (p->tkn->type == TKN_L_CBRACK) {

            // NOTE:: TODO 
        }
        return NULL;
    }
    return NULL;
}

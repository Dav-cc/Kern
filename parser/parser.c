#include "parser.h"

int HAS_MAIN = 0;
int MAX_PARAM_COUNT = 10;

static int peek(Token tkn) { return ((tkn.next)->type); }

static int is_datatype(Token* tkn) {
    return ((tkn->type == TKN_FLOAT) || (tkn->type == TKN_VOID) || (tkn->type == TKN_DOUBLE) ||
            (tkn->type == TKN_CHAR) || (tkn->type == TKN_INT))
               ? 1
               : 0;
}

ASTNode* new_node(int data_size) {
    ASTNode* n = (ASTNode*)malloc(sizeof(ASTNode));
    n->data = malloc(data_size);
    return n;
}

ASTNode* parse_type(Token* tkn) {
    if (is_datatype(tkn)) {
        ASTNode* node = new_node(sizeof(tkn->value));
        node->type = tkn->type;
        node->data = tkn->value;
        return node;
    }
    return NULL;
}
ASTNode* parse_program(Token* token) {
    Token* tkn = token;
    if (is_datatype(tkn)) { // data types (what functions return) soon we support pointers
        if ((peek(*tkn) == TKN_IDENTIFIER)) {
            // tkn = tkn->next;
            if (peek(*tkn->next) == TKN_L_PRAN) {
                // tkn = tkn->next;
                ASTNode* node = parse_function(token);
                return node;
            }
            if ((peek(*tkn->next) == TKN_EQ) || (peek(*tkn) == TKN_SEMCOL)) {
                // tkn = tkn->next;
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
    Token* tkn = token; // tkn->type == datatype func return

    func_dcl* func = (func_dcl*)malloc(sizeof(func_dcl));
    char* function_ID = NULL;
    int param_count = 0;

    ASTNode* return_type = parse_type(tkn);
    if (return_type) {
        tkn = tkn->next;
        function_ID = (char*)tkn->value;
        tkn = tkn->next;
        if (tkn->type == TKN_L_PRAN) {
            if ((peek(*tkn)) != TKN_R_PRAN) {
                tkn = tkn->next;
                ASTNode** params = (ASTNode**)malloc(sizeof(ASTNode*) * 10);
                for (int i = 0; i < 10; i++) {
                    params[i] = NULL;
                }
                while (tkn) {
                    if (tkn->type == TKN_R_PRAN) {
                        tkn = tkn->next;
                        break;
                    }
                    ASTNode* n = parse_type(tkn);
                    if (n) {
                        params[param_count++] = n;
                    }
                    tkn = tkn->next;
                }
                func->fname = function_ID;
                func->param_cont = param_count;
                func->params = params;
                func->return_type = return_type;

                for (int i = 0; i < param_count; i++) {
                    printf("checking param %d...\n", i);
                    fflush(stdout); // مهمه! segfault قبل از این خط یا بعدش؟

                    ASTNode* p = params[i];
                    printf("p = %p\n", p);
                    fflush(stdout);

                    if (p != NULL) {
                        printf("p->type = %d\n", p->type);
                        fflush(stdout);

                        printf("p->data = %p\n", p->data);
                        fflush(stdout);

                        if (p->data != NULL) {
                            printf("param %d: %s\n", i, (char*)p->data);
                        } else {
                            printf("param %d: data is NULL\n", i);
                        }
                    } else {
                        printf("param %d: node is NULL\n", i);
                    }
                    fflush(stdout);
                }
            } else {
                func->param_cont = 0;
                func->params = NULL;
                func->param_cont = 0;
                func->fname = function_ID;
                func->return_type = return_type;
            }
        }
    }
    ASTNode* retnode = (ASTNode*)malloc(sizeof(ASTNode));
    retnode->data = func;
    retnode->type = NUDE_FUNCTION;
    return retnode;
}

ASTNode* parse_var(Token* token) { printf("im in var func\n"); }

ASTNode* parse_block(Token* token) { printf("im in block\n"); }

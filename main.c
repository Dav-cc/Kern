#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include <stdio.h>

int main(int argc, char** argv) {
    const char* source = (argc > 1) ? argv[1] : "t2.c";
    Lexer* lex = src_to_tkn(source);
    Token* tkn = lex->head;
    while (tkn) {
        if (tkn->type == TKN_INT_VALUE) {
            printf("---tkn:INT_VALUE=%d---\n", *(int*)tkn->value);
        } else {
            printf("---tkn:%s---\n", (char*)tkn->value);
        }
        tkn = tkn->next;
    }
    Parser* p;
    p = init_parser(lex->head);
    ASTnode* result = parse_program(p);
    if (result) {
        Program* prog = (Program*)result->data;
        printf("parsed ok: %zu function(s), %zu global variable(s)\n", prog->function_count,
               prog->globvar_count);
    }
}

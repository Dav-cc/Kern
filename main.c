#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include <stdio.h>

int main(int argc, char** argv) {
    Lexer* lex = src_to_tkn("t2.c");
    Token* tkn = lex->head;
    Token* token = lex->head;
    while (tkn) {
        printf("---tkn:%s---\n", (char*)tkn->value);
        tkn = tkn->next;
    }
    Parser* p;
    p = init_parser(tkn);
    parse_program(p);
}

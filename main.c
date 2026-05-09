#include "Lexer/lexer.h"
#include "parser/parser.h"
#include <stdio.h>

int main(int argc, char **argv) {
    Lexer* lex = src_to_tkn("t2.c");
    Token* tkn = lex->head;
    while(tkn){
        printf("---tkn:%s---\n", tkn->value);
        tkn = tkn->next;
    }

    parse_program(lex->head);

}

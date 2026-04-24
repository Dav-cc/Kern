#include "Lexer/lexer.h"
#include <stdio.h>

int main(int argc, char **argv) {
    Lexer* lex = src_to_tkn("test.c");

    Token* tkn = malloc(sizeof(Token));
    tkn = lex->head;

    while(tkn){
        printf("--%s--\n",(char*) tkn->value);
        tkn = tkn->next;
    }
    

}

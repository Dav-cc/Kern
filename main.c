#include "Lexer/lexer.h"
#include <stdio.h>

int main(int argc, char **argv) {
    Lexer* lex = src_to_tkn("test.c");

    Token* tkn = malloc(sizeof(Token));
    tkn = lex->head;

    while(tkn){
        printf("tokne_id = ---%d---, token_vlaue = ---%s---\n",tkn->type, (char*) tkn->value);
        tkn = tkn->next;
    }
    

}

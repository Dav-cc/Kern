#include "Lexer/lexer.h"

int main(int argc, char **argv) {
    Lexer* lex = src_to_tkn("test.c");
    while(lex->head){
        printf("--%s--\n", (char*)lex->head);
        lex->head = lex->head->next;
    }
}

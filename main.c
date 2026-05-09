#include "Lexer/lexer.h"
#include "parser/parser.h"
#include <stdio.h>

int main(int argc, char **argv) {
    Lexer* lex = src_to_tkn("test.c");
    parse_program(lex->head);

}

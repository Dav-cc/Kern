#include "lexer.h"
#include "../utils/u_lexer.h"

Lexer* src_to_tkn(const char* filename){
    long size;
    Lexer* lex = {0};

    char *buffer = read_file(filename, &size);
    lex = lexer_init(filename, buffer, size);

    lex = tokenizer(lex);

    return lex;
}

Lexer* tokenizer(Lexer* l){

}

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>

#include "../Lexer/lexer.h"

char peek(char ch){
    return ch;
}

char is_dig(char ch){ 
    return ((ch <= '9' && ch >= '0') ? ch : -1); 
}

char is_letter(char ch){ 
    return (((ch <= 'Z' && ch >= 'A')|| (ch <= 'z' && ch >= 'a') || ch == '_' )? ch : -1); 
}

char is_dig_letter(char ch){
    return (is_dig(ch) || is_letter(ch)) ? ch : 0;
}

char* read_file(const char* filename, long* len){
    char* buffer;

    FILE* ff = fopen(filename, "r");
    if(ff == NULL){
        fprintf(stderr, "error in openning file\n");
        exit(-1);
    }
    fseek(ff, 0, SEEK_END);
    long length = ftell(ff);
    *len = length;

    buffer = (char*)malloc(length + 1);
    if(buffer == NULL){
        fprintf(stderr, "error in openning file\n");
        exit(-1);
    }
    return buffer;
}


Lexer* lexer_init(const char* filename, char* buffer, long len){
    Lexer* l = (Lexer*)malloc(sizeof(Lexer));
    l->column = 1;
    l->pos = 0;
    l->line = 1;
    l->len = len;
    l->buffer = buffer;
    l->filename = filename;
    l->head = NULL;
    return l;
}


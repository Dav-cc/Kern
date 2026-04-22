#ifndef __u_lex__
#define __u_lex__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../Lexer/lexer.h"

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

#endif

#include "lexer.h"
#include <assert.h>
#include <string.h>


char peek(char ch){
    return ch;
}

char is_dig(char ch){ 
    return ((ch <= '9' && ch >= '0') ? ch : 0); 
}

char is_letter(char ch){ 
    return (((ch <= 'Z' && ch >= 'A')|| (ch <= 'z' && ch >= 'a') || ch == '_' )? ch : 0); 
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
    fseek(ff, 0, SEEK_SET);
    int read = fread(buffer, sizeof(char), length + 1, ff);
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

void skipwhitespaces(Lexer* l){
    while((l->pos < l->len) && (*l->buffer ==' ') && (*l->buffer == '\n')){
        if(*l->buffer == ' '){
            l->buffer++;
            l->pos++;
            l->column++;
        }
        if(*l->buffer == '\n'){
            l->buffer++;
            l->column = 1;
            l->line++;
            l->pos++;
        }
    }
}


Token* _token(token_type type, void* value){
    Token* tkn = (Token*)malloc(sizeof(Token));
    tkn->type = type;
    tkn->value = value;
    tkn->next = NULL;
    return tkn;
}

Lexer* src_to_tkn(const char* filename){
    long size;
    Lexer* lex = {0};

    char *buffer = read_file(filename, &size);
    assert(buffer != NULL);

    lex = lexer_init(filename, buffer, size);
    assert(lex != NULL);

    lex = tokenizer(lex);

    return lex;
}

Lexer* tokenizer(Lexer* l){
    Token* a;
    Token* b;
    Token* head = {0};
    a = make_token(l);
    while(a != NULL){
        printf("im here\n");
        b = make_token(l);
        b = a->next;
        a = b;
    }
    head = a;
    l->head = head;

    return l;
}

Token* make_token(Lexer* l){
    skipwhitespaces(l);

    Token* tkn = make_res_token(l);
    if(tkn == NULL){
        tkn = make_op_token(l);
    }
    if(!tkn){
        printf("error in lexing token\n");
        return NULL;
    }
    return tkn;
}

Token* make_res_token(Lexer* l){
    char c;
    if((c = is_letter(*l->buffer))){
        char* wptr;
        char* word = malloc(101);
        wptr = word;
        *word = c;
        word++;

        while((l->pos < l->len)&& (c = is_dig_letter(*l->buffer))){
            *word = c;
            word++;
            l->buffer++;
        }
        *word = '\0';
        
        if((strcmp(wptr, "int") == 0)){
            return _token(TKN_INT, "#Int_token");
        }
        else if((strcmp(wptr, "main") == 0 )){
            return _token(TKN_MAIN, "#Main_token");
        }
        else if((strcmp(wptr, "char") == 0 )){
            return _token(TKN_CHAR, "#Char_token");
        }
        else if((strcmp(wptr, "float") == 0 )){
            return _token(TKN_FLOAT, "#Float_token");
        }
        else if((strcmp(wptr, "double") == 0 )){
            return _token(TKN_DOUBLE, "#Double_token");
        }
        else if((strcmp(wptr, "return") == 0 )){
            return _token(TKN_RETURN, "#Return_token");
        }
    }
    return NULL;
}

Token* make_op_token(Lexer* l){
    char c;
    if((c = is_dig(*l->buffer)) && l->pos < l->len){
        long value = 0;
        value = c - '0';
        l->buffer++;
        l->column++;
        l->pos++;
        while((c = is_dig(*l->buffer)) && (l->pos < l->len)){
            value *= 10;
            value = c - '0';
            l->buffer++;
            l->pos++;
            l->column++;
        }
    }
    if((c = peek(c)) && (l->pos < l->len)){
        if(c == '{'){
            return _token(TKN_L_CBRACK, "#C_L_brack");
        }
        if(c == '}'){
            return _token(TKN_R_CBRACK, "#C_R_brack");
        }
        if(c == '('){
            return _token(TKN_L_PRAN, "#L_pran");
        }
        if(c == ')'){
            return _token(TKN_R_PRAN, "#R_pran");
        }
        if(c == ';'){
            return _token(TKN_SEMCOL, "#Semi_cl");
        }
        return NULL;
    }
    return NULL;

}

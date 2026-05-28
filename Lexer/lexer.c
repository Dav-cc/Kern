#include "lexer.h"
#include <assert.h>
#include <string.h>

static char is_dig(char ch) { return ((ch <= '9' && ch >= '0') ? ch : 0); }

static char is_letter(char ch) {
    return (((ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a') || ch == '_') ? ch : 0);
}

static char is_dig_letter(char ch) { return (is_dig(ch) || is_letter(ch)) ? ch : 0; }

static char* read_file(const char* filename, long* len) {
    char* buffer = NULL;

    FILE* ff = fopen(filename, "r");
    if (ff == NULL) {
        fprintf(stderr, "error in openning file\n");
        exit(-1);
    }
    fseek(ff, 0, SEEK_END);
    long length = ftell(ff);
    *len = length;

    buffer = (char*)malloc(length + 1); // +1 byte for null termination
    if (buffer == NULL) {
        fprintf(stderr, "error in openning file\n");
        exit(-1);
    }
    fseek(ff, 0, SEEK_SET);
    int read = fread(buffer, sizeof(char), length, ff);
    buffer[read] = '\0';
    fclose(ff);
    return buffer;
}

static Lexer* lexer_init(const char* filename, char* buffer, long len) {
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

static void skipwhitespaces(Lexer* l) {
    while ((l->pos < l->len) && ((*l->buffer == ' ') || (*l->buffer == '\n'))) {
        if (*l->buffer == ' ') {
            l->buffer++;
            l->pos++;
            l->column++;
        }
        if (*l->buffer == '\n') {
            l->buffer++;
            l->column = 1;
            l->line++;
            l->pos++;
        }
    }
}

static Token* _token(token_type type, void* value) {
    Token* tkn = (Token*)malloc(sizeof(Token));
    tkn->type = type;
    tkn->value = value;
    tkn->next = NULL;
    return tkn;
}

Lexer* src_to_tkn(const char* filename) {
    long size;
    Lexer* lex = {0};

    char* buffer = read_file(filename, &size);
    assert(buffer != NULL);

    lex = lexer_init(filename, buffer, size);
    assert(lex != NULL);

    lex = tokenizer(lex);

    return lex;
}

Lexer* tokenizer(Lexer* l) {
    Token* a;
    Token* b;

    a = make_token(l);
    if (!a) {
        return NULL;
    }
    l->head = a;
    while (a->type != TKN_EOF) {
        b = make_token(l);
        a->next = b;
        a = b;
    }

    return l;
}

Token* make_token(Lexer* l) {
    skipwhitespaces(l);

    Token* tkn = make_res_token(l);
    if (tkn == NULL) {
        tkn = make_op_token(l);
    }
    if (!tkn) {
        l->token_count++;
        tkn = _token(TKN_EOF, "#EOF\n");
    }
    return tkn;
}

Token* make_res_token(Lexer* l) {
    char c;
    if ((c = is_letter(*l->buffer))) {
        char* wptr;
        char* word = malloc(101);
        wptr = word;
        *wptr = c;
        wptr++;
        l->buffer++;
        l->pos++;
        l->column++;

        while ((l->pos < l->len) && (c = is_dig_letter(*l->buffer))) {
            *wptr = c;
            wptr++;
            l->buffer++;
            l->pos++;
            l->column++;
        }

        *wptr = '\0';

        if ((strcmp(word, "int") == 0)) {
            l->token_count++;
            return _token(TKN_INT, "#Int_token");
        } else if ((strcmp(word, "while") == 0)) {
            l->token_count++;
            return _token(TKN_WHILE, "#While_token");
        } else if ((strcmp(word, "switch") == 0)) {
            l->token_count++;
            return _token(TKN_SWITCH, "#Switch_token");
        } else if ((strcmp(word, "break") == 0)) {
            l->token_count++;
            return _token(TKN_BREAK, "#Break_token");
        } else if ((strcmp(word, "for") == 0)) {
            l->token_count++;
            return _token(TKN_FOR, "#For_token");
        } else if ((strcmp(word, "continue") == 0)) {
            l->token_count++;
            return _token(TKN_CONTINUE, "#Continue_token");
        } else if ((strcmp(word, "else") == 0)) {
            l->token_count++;
            return _token(TKN_ELSE, "#Else_token");
        } else if ((strcmp(word, "do") == 0)) {
            l->token_count++;
            return _token(TKN_DO, "#Do_token");
        } else if ((strcmp(word, "if") == 0)) {
            l->token_count++;
            return _token(TKN_IF, "#If_token");
        } else if ((strcmp(word, "void") == 0)) {
            l->token_count++;
            return _token(TKN_VOID, "#Void_token");
        } else if ((strcmp(word, "char") == 0)) {
            l->token_count++;
            return _token(TKN_CHAR, "#Char_token");
        } else if ((strcmp(word, "float") == 0)) {
            l->token_count++;
            return _token(TKN_FLOAT, "#Float_token");
        } else if ((strcmp(word, "double") == 0)) {
            l->token_count++;
            return _token(TKN_DOUBLE, "#Double_token");
        } else if ((strcmp(word, "return") == 0)) {
            l->token_count++;
            return _token(TKN_RETURN, "#Return_token");
        } else if ((is_dig_letter(*word))) {
            l->token_count++;
            return _token(TKN_IDENTIFIER, (void*)word);
        }
    }
    return NULL;
}

Token* make_op_token(Lexer* l) {
    char c;
    if ((c = is_dig(*l->buffer)) && (l->pos < l->len)) {
        long value = c - 48;
        l->buffer++;
        l->column++;
        l->pos++;
        while ((c = is_dig(*l->buffer)) && (l->pos < l->len)) {
            value *= 10;
            value += c - 48;
            l->buffer++;
            l->pos++;
            l->column++;
        }
        int* fval = (int*)malloc(sizeof(int));
        *fval = value;
        l->token_count++;
        return _token(TKN_INT_VALUE, (void*)fval);
    }

    // TODO : string litterals

    if ((c = *l->buffer) && (l->pos < l->len)) {
        if (c == '{') {
            l->buffer++;
            l->column++;
            l->pos++;
            l->token_count++;
            return _token(TKN_L_CBRACK, "#C_L_brack");
        }
        if (c == '}') {
            l->buffer++;
            l->column++;
            l->pos++;
            l->token_count++;
            return _token(TKN_R_CBRACK, "#C_R_brack");
        }
        if (c == '(') {
            l->buffer++;
            l->column++;
            l->pos++;
            l->token_count++;
            return _token(TKN_L_PRAN, "#L_pran");
        }
        if (c == ')') {
            l->buffer++;
            l->column++;
            l->pos++;
            l->token_count++;
            return _token(TKN_R_PRAN, "#R_pran");
        }
        if (c == ';') {
            l->buffer++;
            l->column++;
            l->pos++;
            l->token_count++;
            return _token(TKN_SEMCOL, "#Semi_cl");
        }
        if (c == '=') {
            l->buffer++;
            l->column++;
            l->pos++;
            l->token_count++;
            return _token(TKN_EQ, "#EQ");
        }
        if (c == '+') {
            l->buffer++;
            l->column++;
            l->pos++;
            l->token_count++;
            return _token(TKN_PLUS, "#PULS");
        }
        if (c == '-') {
            l->buffer++;
            l->column++;
            l->pos++;
            l->token_count++;
            return _token(TKN_MINUS, "#MINUS");
        }
        if (c == ',') {
            l->buffer++;
            l->column++;
            l->pos++;
            l->token_count++;
            return _token(TKN_COMMA, "#COMMA");
        }
        return NULL;
    }
    return NULL;
}

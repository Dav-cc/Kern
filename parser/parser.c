#include "parser.h"

static token_type peek(Parser* p) { return p->cur->type; }

static Token* advance(Parser* p) {
    p->prev = p->cur;
    if (p->cur->type != TKN_EOF)
        p->cur = (p->cur)->next;
    return p->cur;
}



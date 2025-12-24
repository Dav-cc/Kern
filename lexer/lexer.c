#include "lexer.h"



token* next_token(char* buf){
   char* buf_ptr = buf;
   char* b_r = buf_ptr;



}


token* token_stream(char* buffer){
   char* buffer_ptr = buffer;
   token* ret = next_token(buffer_ptr);
   token* h = ret;
   token* t = h;

   while(ret){
      h = next_token(buffer_ptr);
      h = h->next;
      ret = h;
   }
   return t;
}

token* new_tkn(Token_type type, void* data){
    token* new = (token*)malloc(sizeof(token));
    new->type = type;
    new->value = data;
    new->next = NULL;
    return new;
}

token* tokenize(char* path){
   void* file = NULL;
   file = fopen(path, "rb");
   if(!file){
       fprintf(stderr, "err in openning fopen\n");
   }
   char* buffer = malloc(1024);
   int byr = fread(buffer, sizeof(char), 1024, file);
   buffer[byr] = '\0';

   token* ret = token_stream(buffer);
   return ret;
}
